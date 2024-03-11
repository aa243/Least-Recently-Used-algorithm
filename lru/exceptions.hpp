#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

namespace sjtu
{

	class exception
	{
	protected:
		std::string variant = "";
		std::string detail = "";

	public:
		exception() {}
		exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}
		virtual std::string what()
		{
			return variant + " " + detail;
		}
	};

	class index_out_of_bound : public exception
	{
		/* __________________________ */
		index_out_of_bound()
		{
			variant = "index_out_of_bound";
			detail = "";
		}
		index_out_of_bound(std::string s)
		{
			variant = "index_out_of_bound";
			detail = s;
		}
	};

	class runtime_error : public exception
	{
		/* __________________________ */
		runtime_error()
		{
			variant = "runtime_error";
			detail = "";
		}
		runtime_error(std::string s)
		{
			variant = "runtime_error";
			detail = s;
		}
	};

	class invalid_iterator : public exception
	{
		/* __________________________ */
		invalid_iterator()
		{
			variant = "invalid_iterator";
			detail = "";
		}
		invalid_iterator(std::string s)
		{
			variant = "invalid_iterator";
			detail = s;
		}
	};

	class container_is_empty : public exception
	{
		/* __________________________ */
		container_is_empty()
		{
			variant = "container_is_empty";
			detail = "";
		}
		container_is_empty(std::string s)
		{
			variant = "container_is_empty";
			detail = s;
		}
	};
}

#endif
