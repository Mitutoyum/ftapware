#pragma once

#include <mutex>
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <utility>
#include <optional>
#include <exception>
#include <functional>
#include <shared_mutex>
#include <condition_variable>

#include <iostream>

namespace FTAPWare::Cache {
    template <typename T, typename Clock = std::chrono::steady_clock>
    class LazyCache {
    private:
        using duration = typename Clock::duration;
        using time_point = std::chrono::time_point<Clock>;
        using builder_t = std::function<T()>;

        mutable std::shared_mutex rw_mutex;
        mutable std::mutex wait_mutex;
        mutable std::mutex build_mutex;
        mutable std::mutex worker_mutex;

        duration ttl;
        builder_t builder;
        std::shared_ptr<const T> value;
        time_point last_refresh = time_point::min();
        std::optional<std::exception_ptr> last_error;

        bool rebuilding = false;
        bool pending_refresh = false;

        std::jthread worker;
        std::condition_variable cv_worker;
        mutable std::condition_variable cv_waiter;
            
        void SetupWorker(std::stop_token st) {
            std::unique_lock<std::mutex> lock(worker_mutex);

            while (!st.stop_requested()) {
                cv_worker.wait(lock, [&] { return st.stop_requested() || pending_refresh; });

                if (st.stop_requested()) break;

                pending_refresh = false;
                lock.unlock();
                RefreshSync();
                lock.lock();
            }
        }
    public:
        LazyCache(duration ttl, builder_t builder) : ttl(ttl), builder(std::move(builder)),  worker([this](std::stop_token st) {
            SetupWorker(st);
        }) {}

        ~LazyCache() {
            worker.request_stop();
            cv_worker.notify_all();
        }

        std::shared_ptr<const T> Get() {
            {
                std::shared_lock read_lock(rw_mutex);

                if (value) {
                    if ((Clock::now() - last_refresh) >= ttl)
                        RefreshAsync();

                    return value;
                }
            }

            RefreshSync();

            std::shared_lock read_lock(rw_mutex);
            return value;
        }

        void RefreshSync() {
            std::unique_lock build_lock(build_mutex);
            builder_t builder_copy;

            {
                std::shared_lock read_lock(rw_mutex);
                if (value && (Clock::now() - last_refresh) < ttl)
                    return;

                builder_copy = builder;
            }

            {
                std::unique_lock wait_lock(wait_mutex);
                rebuilding = true;
            }

            try {
                T fresh = builder_copy();

                auto new_value = std::make_shared<const T>(std::move(fresh));

                {
                    std::unique_lock write_lock(rw_mutex);
                    value = std::move(new_value);
                    last_refresh = Clock::now();
                    last_error.reset();
                }
            }
            catch (...) {
                {
                    std::unique_lock write_lock(rw_mutex);
                    last_error = std::current_exception();
                }
            }

            {
                std::unique_lock wait_lock(wait_mutex);
                rebuilding = false;
            }

            cv_waiter.notify_all();
        }

        void RefreshAsync() {
            {
                std::unique_lock lock(worker_mutex);

                if (pending_refresh)
                    return;

                pending_refresh = true;
            }

            cv_worker.notify_one();
        }

        bool WaitForRefresh(std::optional<std::chrono::milliseconds> timeout = std::nullopt) const {
            std::unique_lock lock(wait_mutex);

            if (timeout) {
                return cv_waiter.wait_for(lock, *timeout, [&] {
                    return !rebuilding;
                });
            }

            cv_waiter.wait(lock, [&] {
                return !rebuilding;
            });

            return true;
        }

        bool IsStale() const {
            std::shared_lock lock(rw_mutex);

            return !value || (Clock::now() - last_refresh) >= ttl;
        }

        bool IsRebuilding() const {
            std::unique_lock lock(wait_mutex);
            return rebuilding;
        }

        time_point GetLastRefresh() const {
            std::shared_lock lock(rw_mutex);
            return last_refresh;
        }

        void SetTimeToLive(duration ttl) {
            std::unique_lock lock(rw_mutex);
            this->ttl = ttl;
        }

        duration GetTimeToLive() const {
            std::shared_lock lock(rw_mutex);
            return ttl;
        }

        void SetBuilder(builder_t builder) {
            std::unique_lock lock(rw_mutex);
            this->builder = std::move(builder);
        }

        std::optional<std::exception_ptr> GetLastError() const {
            std::shared_lock lock(rw_mutex);
            return last_error;
        }
    };
}