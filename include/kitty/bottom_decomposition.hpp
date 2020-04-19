#pragma once
#include <cstdint>
#include <vector>
#include "kitty.hpp"
#include "dynamic_truth_table.hpp"
#include "constructors.hpp"
#include "decomposition.hpp"
#include "operators.hpp"
#include "print.hpp"
#include "top_decomposition.hpp"


namespace kitty
{
	template<class TT>
	bool is_bottom_decomposition(const TT&  tt)
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
		std::vector<kitty::bottom_decomposition> type;
		for (auto j = 1u; j < support.size(); ++j)
		{
			for (auto i = 0u; i < j; ++i)
			{
				auto res = kitty::is_bottom_decomposable(tt, support[i], support[j]);
				if (res != kitty::bottom_decomposition::none)
				{
					return true;
				}
				else
				{
					type.push_back(res);
				}
			}
		}
		uint64_t num{ 0 };
		for (auto i = 0; i < type.size(); ++i)
		{
			if (type[i] == kitty::bottom_decomposition::none)
				num++;
		}
		if (num == type.size())
		{
			return false;
		}
	}

	template<class TT>
	std::vector<std::tuple<std::pair<int, int>, std::string, TT>> bottom_decomposition_impl(const TT& func)
	{
		std::vector<std::tuple<std::pair<int, int>, std::string, TT>> result;
		bool is_end{ false };
		auto tt = func;
		auto support = kitty::exist_var_tt(tt);
		auto var_size = support.size();
		if (kitty::is_bottom_decomposition(tt) == false)
		{
			std::cout << "[i]. the function is not decomposition" << std::endl;

		}
		else
		{
			for (auto k = 1; k <= var_size; k++)
			{
				if (kitty::exist_var_tt(tt).size() == 1) break;

				for (auto j = 1u; j < var_size; ++j)
				{
					if (is_end)
					{
						is_end = false;
						break;
					}
					else
					{
						for (auto i = 0u; i < j; ++i)
						{
							auto res = kitty::is_bottom_decomposable(tt, support[i], support[j], &tt);

							if (res != kitty::bottom_decomposition::none)
							{
								is_end = true;

								switch (res)
								{
								default:
									continue;
								case kitty::bottom_decomposition::and_:
									result.push_back(std::make_tuple(std::make_pair(support[i], support[j]), "and", tt));
									break;
								case kitty::bottom_decomposition::or_:
									result.push_back(std::make_tuple(std::make_pair(support[i], support[j]), "or", tt));
									break;
								case kitty::bottom_decomposition::lt_:
									result.push_back(std::make_tuple(std::make_pair(support[i], support[j]), "lt", tt));
									break;
								case kitty::bottom_decomposition::le_:
									result.push_back(std::make_tuple(std::make_pair(support[i], support[j]), "le", tt));
									break;
								case kitty::bottom_decomposition::xor_:
									result.push_back(std::make_tuple(std::make_pair(support[i], support[j]), "xor", tt));
									break;
								}
								support.erase(support.begin() + j);
								break;
							}
						}
					}

				}
			}
		}
		return result;
	}


	using namespace std;
	template<class TT>
	void print_bottom_decomposed_information(TT const& tt)
	{
		auto result = bottom_decomposition_impl(tt);
		for (int i = 0; i < result.size(); i++)
		{
			cout << "[" << i << "]";
			auto t1 = std::get<0>(result[i]);
			cout << "  first index = " << t1.first <<", second index = "<<t1.second;

			auto t2 = std::get<1>(result[i]);
			cout << ", operational type =  " << t2;			

			auto t3 = std::get<2>(result[i]);
			cout << ", the remainder tt is ";
			print_hex(t3);
			cout << endl;
		}
	}


}
