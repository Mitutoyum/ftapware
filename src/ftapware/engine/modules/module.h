#pragma once

#include <string>

namespace FTAPWare {
	class Context;
}

namespace FTAPWare::Modules {
	class Module {
	protected:
		const Context& context;
	public:
		Module(const Context& context);
		virtual ~Module() = default;
		virtual std::string GetName() const = 0;
	};
}