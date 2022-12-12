#include <improc/services/pipes/base_pipe.hpp>

#include <pipes/pipes.hpp>
#include <memory>
#include <any>

#include <algorithm>
#include <functional>
#include <iostream>
// #include <execution>

double transform_increment(int number)
{
    return static_cast<double>(number + 1);
}

bool bigger_5(double number)
{
    return number > 5;
}

class IntClass
{
    private:
        int data_;

    public:
        IntClass() : data_(0) {};
        explicit IntClass(int number) : data_(number) {};

        double double_increment()
        {
            return static_cast<double>(this->data_ + 2);
        }
};

template <class OutputType, class InputType, class UnaryOperator>
std::vector<OutputType> Transform(const std::vector<InputType>& input_vector, const UnaryOperator& transform_functor)
{
    std::vector<OutputType> transform_res(input_vector.size());
    std::transform  ( input_vector.begin(),input_vector.end(),transform_res.begin()
                    , transform_functor );
    return transform_res;
}

int main()
{
    std::vector<int> original {1,2,3,4,5,6,7,8,9,10};
    std::vector<double> res_transf_function(original.size());
    std::transform  ( original.begin(), original.end(), res_transf_function.begin()
                    , std::function<double(int)> {&transform_increment} );
    std::cout << "With Function: ";
    for (double number: res_transf_function)
    {
        std::cout << std::to_string(number) << "; ";
    }
    std::cout << std::endl;

    std::vector<IntClass> original_class {IntClass(1),IntClass(2),IntClass(3),IntClass(4),IntClass(5),IntClass(6),IntClass(7),IntClass(8),IntClass(9),IntClass(10)};
    std::vector<double> res_transf_mem_fn(original.size());
    // std::transform  ( std::execution::par_unseq, original_class.begin(), original_class.end(), res_transf_mem_fn.begin()
    std::transform  ( original_class.begin(), original_class.end(), res_transf_mem_fn.begin()
                    , std::mem_fn(&IntClass::double_increment) );

    std::cout << "With Mem_Fn: ";
    for (double number: res_transf_mem_fn)
    {
        std::cout << std::to_string(number) << "; ";
    }
    std::cout << std::endl;

    std::vector<double> filter_copy {};
    std::copy_if    ( res_transf_function.begin(), res_transf_function.end(), std::back_inserter(filter_copy)
                    , std::function<bool(double)> {&bigger_5} );
    std::cout << "Copy If: ";
    for (double number: filter_copy)
    {
        std::cout << std::to_string(number) << "; ";
    }
    std::cout << std::endl;

    std::vector<double> filter_remove_copy {};
    std::remove_copy_if ( res_transf_function.begin(), res_transf_function.end(), std::back_inserter(filter_remove_copy)
                        , std::function<bool(double)> {&bigger_5} );
    std::cout << "Remove Copy If: ";
    for (double number: filter_remove_copy)
    {
        std::cout << std::to_string(number) << "; ";
    }
    std::cout << std::endl;

    std::vector<double>::iterator filter_remove {};
    filter_remove = std::remove_if  ( res_transf_function.begin(), res_transf_function.end()
                                    , std::function<bool(double)> {&bigger_5} );
    std::cout << "Remove If: ";
    for (std::vector<double>::iterator number_iter = res_transf_function.begin(); number_iter != filter_remove; ++number_iter)
    {
        std::cout << std::to_string(*number_iter) << "; ";
    }
    std::cout << std::endl;

    std::vector<int>    original_data {1,2,3,4,5,6,7,8,9,10};
    std::vector<double> res_action = Transform<double>(original_data,std::function<double(int)> {&transform_increment});

    std::vector<double> res_pipes {};
    // original_data   >>= pipes::transform(std::function<double(int)> {&transform_increment})
    //                 >>= pipes::filter(std::function<bool(double)> {&bigger_5})
    //                 >>= pipes::push_back(res_pipes);

    auto pipeline   = pipes::transform(std::function<double(int)> {&transform_increment})
                    >>= pipes::filter(std::function<bool(double)> {&bigger_5});
    original_data >>= pipeline >>= pipes::push_back(res_pipes);
    std::cout << "Pipes: ";
    for (double number: res_pipes)
    {
        std::cout << std::to_string(number) << "; ";
    }
    std::cout << std::endl;

    std::vector<double> improc_result {};
    auto improc_pipeline =      improc::TransformPipe(std::function<double(int)> {&transform_increment})
                            >>= improc::FilterPipe(std::function<bool(double)> {&bigger_5});
    original_data >>= improc_pipeline >>= pipes::push_back(improc_result);
    std::cout << "Improc Pipes: ";
    for (double number: improc_result)
    {
        std::cout << std::to_string(number) << "; ";
    }
    std::cout << std::endl;
    return 0;
}