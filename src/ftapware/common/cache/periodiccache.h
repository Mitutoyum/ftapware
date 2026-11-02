#pragma once

namespace FTAPWare::Cache {
    //template <typename T, typename Clock = std::chrono::steady_clock>
//class PeriodicCache {
//private:
//    using duration = typename Clock::duration;
//    using time_point = std::chrono::time_point<Clock>;
//    using builder_t = std::function<T()>;
//
//    mutable std::shared_mutex _rw_mutex;
//    mutable std::mutex _wait_mutex;
//    mutable std::mutex _build_mutex;
//    mutable std::mutex _worker_mutex;
//
//    std::shared_ptr<const T> _value;
//    duration _ttl;
//    builder_t _builder;
//    time_point _last_refresh;
//    int _current_epoch = 0;
//    bool _rebuilding = false;
//    bool _pending_refresh = false;
//
//    mutable std::condition_variable _cv_waiter;
//    std::condition_variable _cv_worker;
//    std::optional<std::exception_ptr> _last_error;
//
//    std::jthread _worker;
//
//    const TeleportHandler* _teleport_handler;
//
//    void SetupWorker(std::stop_token st) {
//        std::unique_lock<std::mutex> worker_lock(_worker_mutex);
//        while (!st.stop_requested()) {
//            auto next = Clock::now() + _ttl;
//
//            _cv_worker.wait_until(worker_lock, next, [&] {
//                return st.stop_requested() || _pending_refresh;
//                });
//            if (st.stop_requested()) break;
//
//            if (!_teleport_handler->IsGameReady()) {
//                continue;
//            }
//
//            worker_lock.unlock();
//            RefreshSync();
//            worker_lock.lock();
//            pending_refresh = false;
//        }
//    }
//    
//public:
//    PeriodicCache(const TeleportHandler* teleport_handler, duration ttl, builder_t builder)
//        : _teleport_handler(teleport_handler), _ttl(ttl), _builder(std::move(builder)), _last_refresh((time_point::min)()) {
//        _worker = std::jthread([this](std::stop_token st) { SetupWorker(st); });
//    }
//    ~PeriodicCache() = default;
//
//
//    void Stop() {
//        _worker.request_stop();
//    }
//
//    std::shared_ptr<const T> Get() const {
//        int epoch = _teleport_handler->GetEpoch();
//
//        {
//            std::shared_lock read_lock(_rw_mutex);
//            if (_value && _current_epoch == epoch)
//                return _value;
//        }
//
//        RefreshSync();
//
//        std::shared_lock read_lock(_rw_mutex);
//        return _value;
//    }
//
//    void RefreshSync() {
//        std::unique_lock build_lock(_build_mutex);
//
//        int epoch = _teleport_handler->GetEpoch();
//        builder_t builder_copy;
//
//        {
//            std::shared_lock read_lock(_rw_mutex);
//            if (_value && _current_epoch == epoch)
//                return;
//
//            builder_copy = _builder;
//        }
//
//        {
//            std::unique_lock wait_lock(_wait_mutex);
//            _rebuilding = true;
//        }
//
//        try {
//            T fresh = builder_copy();
//
//            std::unique_lock write_lock(_rw_mutex);
//            _value = std::make_shared<const T>(std::move(fresh));
//            _last_refresh = Clock::now();
//            _last_error.reset();
//            _current_epoch = epoch;
//        }
//        catch (...) {
//            std::unique_lock write_lock(_rw_mutex);
//            _last_error = std::current_exception();
//        }
//
//        {
//            std::unique_lock wait_lock(_wait_mutex);
//            _rebuilding = false;
//            _cv_waiter.notify_all();
//        }
//    }
//
//    void RefreshAsync() {
//        std::unique_lock write_lock(_worker_mutex);
//
//        if (_pending_refresh)
//            return;
//
//        _pending_refresh = true;
//        _cv_worker.notify_one();
//    }
//
//    bool WaitForRefresh(std::optional<std::chrono::milliseconds> timeout = std::nullopt) const {
//        std::unique_lock wait_lock(_wait_mutex);
//
//        if (!_rebuilding)
//            return true;
//
//        if (timeout) {
//            return _cv_waiter.wait_for(wait_lock, *timeout, [&] {
//                return !_rebuilding;
//                });
//        }
//        else {
//            _cv_waiter.wait(wait_lock, [&] {
//                return !_rebuilding;
//                });
//            return true;
//        }
//    }
//
//    bool Rebuilding() const {
//        std::unique_lock wait_lock(_wait_mutex);
//        return _rebuilding;
//    }
//
//    time_point LastRefresh() const {
//        std::shared_lock read_lock(_rw_mutex);
//        return _last_refresh;
//    }
//
//    void SetTimeToLive(duration ttl) {
//        std::unique_lock write_lock(_rw_mutex);
//        _ttl = ttl;
//    }
//
//    duration TimeToLive() const {
//        std::shared_lock read_lock(_rw_mutex);
//        return _ttl;
//    }
//
//    void SetBuilder(builder_t builder) {
//        std::unique_lock write_lock(_rw_mutex);
//        _builder = std::move(builder);
//    }
//
//    std::optional<std::exception_ptr> LastError() const {
//        std::shared_lock read_lock(_rw_mutex);
//        return _last_error;
//    }
//};

}