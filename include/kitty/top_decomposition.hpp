#pragma once
#include <cstdint>
#include <vector>
#include "kitty.hpp"
#include "dynamic_truth_table.hpp"
#include "constructors.hpp"
#include "decomposition.hpp"
#include "operators.hpp"
#include "print.hpp"


namespace kitty
{
	template <class TT>
	std::vector<int>  exist_var_tt(const TT& tt)
	{
		std::vector<int> support;
		for (auto i = 0; i < tt.num_vars(); ++i)
		{
			if (kitty::has_var(tt, i))
			{
				support.push_back(i);
			}
		}
		return support;
	}

	template<class TT>
	bool is_top_decomposition(const TT& tt)
	{
		if (kitty::is_const0(tt))
		{
			return false;
		}
		if (kitty::is_const0(~tt))
		{
			return false;
		}
		auto support = kitty::exist_var_tt(tt);
		std::vector<kitty::top_decomposition> type;
		for (auto i = 0; i < support.size(); i++)
		{
			auto var_index = support[i];
			auto res = kitty::is_top_decomposable(tt, var_index);
			if (res != kitty::top_decomposition::none)
			{
				return true;
			}
			else
			{
				type.push_back(res);
			}
		}
		uint64_t num{ 0 };
		for(auto i=0;i<type.size();++i)
		{
			if (type[i] == kitty::top_decomposition::none)
				num++;
		}
		if (num == type.size())
		{
			return false;
		}
	}

	template <class TT>
	std::vector<std::tuple<int, std::string, TT>> top_decomposition_impl(const TT&  func)
	{
		std::vector<std::tuple<int, std::string, TT>> result;
		TT func1, tt;
		tt = func;
		bool is_end{ false };
		auto support = kitty::exist_var_tt(tt);
		if (kitty::is_top_decomposition(tt) == false)
		{
			std::cout << "[i]. the function is not decomposition" << std::endl;
		}
		else
		{
			for (auto j = 1; j <= support.size(); j++)
			{
				TT remainder;
				if (is_const0(tt)) break;
				for (auto i = 0; i < support.size(); i++)
				{
					auto var_index = support[i];
					auto res = kitty::is_top_decomposable(tt, var_index);
					if (res == kitty::top_decomposition::none)
					{
						remainder = tt;
					}
					else
					{
						remainder = cofactor0(tt, var_index);
						tt = remainder;

						switch (res)
						{
						default:
							assert(false);
						case kitty::top_decomposition::and_:
							result.push_back(std::make_tuple(support[i], "and", remainder));
							break;
						case kitty::top_decomposition::or_:
							result.push_back(std::make_tuple(support[i], "or", remainder));
							break;
						case kitty::top_decomposition::lt_:
							result.push_back(std::make_tuple(support[i], "lt", remainder));
							break;
						case kitty::top_decomposition::le_:
							result.push_back(std::make_tuple(support[i], "le", remainder));
							break;
						case kitty::top_decomposition::xor_:
							result.push_back(std::make_tuple(support[i], "xor", remainder));
							break;
						}
						break;
					}
				}
			}
			return result;
		}
	}


	using namespace std;
	template<class TT>
	void print_top_decomposed_information(TT const& tt)
	{
		auto result = top_decomposition_impl(tt);
		for (int i = 0; i < result.size(); i++)
		{
			cout << "[" << i << "]";
			auto t1 = std::get<0>(result[i]);
			cout << "  index = " << t1;

			auto t2 = std::get<1>(result[i]);
			cout << ", operational type =  " << t2;

			auto t3 = std::get<2>(result[i]);
			cout << ", the remainder tt is ";
			print_hex(t3);
			cout << endl;
		}
	}


}
