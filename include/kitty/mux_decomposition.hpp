#pragma once
#include <cstdint>
#include <vector> 
#include "kitty/kitty.hpp"

namespace kitty
{

	// remainder truth table
	template<class TT>
	TT remainder(const TT& tt, uint32_t data_var_index1, uint32_t data_var_index2, uint32_t sel_var_index)
	{
		//F(x_i=1,x_j=1)
		auto then_ = fixed_double_data_var_value_tt(tt, data_var_index1, data_var_index2, true);
		//F(x_i=0,x_j=0)
		auto else_ = fixed_double_data_var_value_tt(tt, data_var_index1, data_var_index2, false);

		// which 
		if (data_var_index1 > data_var_index2)
		{
			std::swap(data_var_index1, data_var_index2);
		}
		if (data_var_index2 > sel_var_index)
		{
			std::swap(data_var_index2, sel_var_index);
		}
		if (data_var_index1 > data_var_index2)
		{
			std::swap(data_var_index1, data_var_index2);
		}
		auto index = data_var_index1;

		return mux_var(index, then_, else_);
	}

	//F(x_i = 0/1 ) or F(x_j = 0/1 ) or F(x_s = 0/1 )
	template<class TT>
	TT fixed_single_data_var_value_tt(const TT& tt, uint32_t var_index, bool var_value)
	{
		if (var_value)
		{
			return cofactor1(tt, var_index);
		}
		else
		{
			return cofactor0(tt, var_index);
		}
	}

	//F(x_i = 0/1,x_j = 0/1 )
	template<class TT>
	TT fixed_double_data_var_value_tt(const TT& tt, uint32_t data_var_index1, uint32_t data_var_index2, bool var_value)
	{
		if (var_value )
		{
			auto c1 = fixed_single_data_var_value_tt(tt, data_var_index2, true);
			auto c1b1 = fixed_single_data_var_value_tt(c1, data_var_index1, true);
			return c1b1;
		}
		else
		{
			auto b0 = fixed_single_data_var_value_tt(tt, data_var_index1,false);
			auto b0c0 = fixed_single_data_var_value_tt(b0, data_var_index2, false);
			return b0c0;
		}
	}



	//F_x_i or F_x_j or F_x_s
	template<class TT>
	TT fixed_single_data_var_tt(const TT& tt, uint32_t var_index)
	{
		auto f0 = fixed_single_data_var_value_tt(tt, var_index, false);
		auto f1 = fixed_single_data_var_value_tt(tt, var_index, true);
		return binary_xor(f0, f1);
	}

	//F_x_i_x_j
	template<class TT>
	TT fixed_double_data_var_tt(const TT& tt, uint32_t data_var_index1, uint32_t data_var_index2)
	{
		auto f_x_i_xj0 = fixed_single_data_var_value_tt(fixed_single_data_var_tt(tt, data_var_index1), data_var_index2, false);
		auto f_x_i_xj1 = fixed_single_data_var_value_tt(fixed_single_data_var_tt(tt, data_var_index1), data_var_index2, true);
		auto f_x_i_x_j = binary_xor(f_x_i_xj0,f_x_i_xj1);
		return f_x_i_x_j;
	}

	template <class TT>
	bool is_equal(TT const& tt, uint32_t data_var_index1, uint32_t data_var_index2, uint32_t sel_var_index)
	{
		auto var_size = tt.num_vars();

		kitty::dynamic_truth_table xi(var_size), xj(var_size), xk(var_size);
		create_nth_var(xi, data_var_index1);
		create_nth_var(xj, data_var_index2);
		create_nth_var(xk, sel_var_index);

		//(~x_s /\ x_i ) \/ (x_s /\ x_j ) = z1
		auto xki = binary_and(unary_not(xk), xi);
		auto xkj = binary_and(xk, xj);
		auto mux = binary_or(xki, xkj);

		//f_x_i(x_s=0)
		auto f_x_i_s0 = fixed_single_data_var_value_tt(fixed_single_data_var_tt(tt, data_var_index1), sel_var_index, false);
		auto fx_ij0 = fixed_double_data_var_value_tt(tt, data_var_index1, data_var_index2, false); // f(x_i=0,x_j=0)
		
		auto hz = binary_and(f_x_i_s0, mux); // z1 * F_x_i(x_s=0)
		auto func = binary_xor(hz, fx_ij0);//F(x_i=0,x_j=0)^(z1*F_x_i(x_s=0) )
		 

		if (equal(tt, func))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//special case  ab^d+!ac^d
	template<class TT>
	bool is_special_case(const TT& tt, uint32_t data_var_index1, uint32_t data_var_index2, uint32_t sel_var_index)
	{
		auto var_size = tt.num_vars();

		kitty::dynamic_truth_table xi(var_size), xj(var_size), xk(var_size);
		create_nth_var(xi, data_var_index1);
		create_nth_var(xj, data_var_index2);
		create_nth_var(xk, sel_var_index);

		auto f_data_var00 = fixed_double_data_var_value_tt(tt, data_var_index1, data_var_index2, false); //d
		auto t1 = binary_xor(binary_and(xk, xj), f_data_var00); //ab^d
		auto t2 = binary_xor(binary_and(unary_not(xk),xi),f_data_var00); //!ac^d
		auto func = binary_or(t1, t2); //ab^d+!ac^d

		if (equal(tt, func))
		{
			return true;
		}
		else
		{
			return false;
		}

	}


	template<class TT>
	std::pair<bool, std::tuple<uint32_t, uint32_t, uint32_t>> is_mux_decomposition(const TT& tt)
	{
		std::pair<bool, std::tuple<uint32_t, uint32_t, uint32_t>> res;
		if (kitty::is_const0(tt))
		{
			res = std::make_pair(false, std::make_tuple(0, 0, 0));
			return res;
		}
		if (kitty::is_const0(~tt))
		{
			res = std::make_pair(false, std::make_tuple(0, 0, 0));
			return res;
		}

		std::vector<int> support;
		support = exist_var_tt(tt);
		TT condition = tt.construct();

		for (auto k = 0; k < exist_var_tt(tt).size(); ++k)
		{
			for (auto j = 0; j < exist_var_tt(tt).size(); ++j)
			{
				for (auto i = 0; i < exist_var_tt(tt).size(); ++i)
				{
					if ((i != j) && (i != k) && (j != k))
					{
						//F_x_i or F_x_j
						auto f_x_i = fixed_single_data_var_tt(tt, support[i]);
						auto f_x_j = fixed_single_data_var_tt(tt, support[j]);

						//F_x_i_x_j
						auto f_x_i_x_j = fixed_double_data_var_tt(tt, support[i], support[j]);

						//F_x_i(x_s=0) or F_x_i(x_s=1)
						auto f_x_i_s1 = fixed_single_data_var_value_tt(f_x_i, support[k], true);
						auto f_x_j_s0 = fixed_single_data_var_value_tt(f_x_j, support[k], false);

						//F(x_i=0,x_j=0) or F(x_i=1,x_j=1)
						auto fx_ij0 = fixed_double_data_var_value_tt(tt, support[i],support[j], false);
						auto fx_ij1 = fixed_double_data_var_value_tt(tt, support[i], support[j], true);

						// F(x_i=0,x_j=0)_x_s or F(x_i=1,x_j=1)_x_s
						auto fx_ij0_x_s = fixed_single_data_var_tt(fx_ij0, support[k]);
						auto fx_ij1_x_s = fixed_single_data_var_tt(fx_ij1, support[k]);

						if (!equal(f_x_i, f_x_j) && equal(f_x_i_s1, condition) && equal(f_x_j_s0, condition) && equal(fx_ij0_x_s, condition) && equal(fx_ij1_x_s, condition) && equal(f_x_i_x_j, condition))
						{
								res = std::make_pair(true, std::make_tuple(support[i], support[j], support[k]));
							
						}

					}

				}
			}
		}
		return res;
	}

	template<class TT>
	std::vector<std::tuple<uint32_t, uint32_t, uint32_t, TT>> mux_decomposition_impl(TT func)
	{
		std::vector<std::tuple<uint32_t, uint32_t, uint32_t, TT>>  result;

		while (is_mux_decomposition(func).first)
		{
			auto i = std::get<0>(is_mux_decomposition(func).second);
			auto j = std::get<1>(is_mux_decomposition(func).second);
			auto k = std::get<2>(is_mux_decomposition(func).second);
			auto rem = remainder(func, i, j, k);
			result.push_back(std::make_tuple(i, j, k, rem));
			func = rem;
		}
		return result;
	}

	using namespace std;
	template<class TT>
	void print_mux_decomposed_information(TT const& tt)
	{
		auto result = mux_decomposition_impl(tt);
		for (int i = 0; i < result.size(); i++)
		{
			cout << "[" << i << "]";
		   auto t1 = std::get<0>(result[i]);
			cout << "  x_i = " << t1 ;

			auto t2 = std::get<1>(result[i]);
			cout << "  x_j = " << t2 ;

			auto t3 = std::get<2>(result[i]);
			cout << "  x_s = " << t3 ;

			auto t4 = std::get<3>(result[i]);
			cout << "  the remainder tt is ";
			print_hex(t4);
			cout << endl;
		}
	}

}


