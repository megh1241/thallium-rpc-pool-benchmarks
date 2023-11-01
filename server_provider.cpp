#include <iostream>
#include <thallium.hpp>
#include <thallium/serialization/stl/string.hpp>
#include <unistd.h>
#include <random>
#include <chrono>


namespace tl = thallium;

class my_sum_provider : public tl::provider<my_sum_provider> {
	private:

		int cpu_intensive_task(){
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> distr(0, 6);
			int large_number = 5000;
			int sum = 0;
			for (int i=0; i<large_number*large_number; ++i){
				sum += distr(gen);
			}
			return sum;
		}
		void prod(const tl::request& req, int x, int y) {
			auto result = cpu_intensive_task();
			req.respond(x*y + result);
		}
		int sum(int x, int y) {
			auto result = cpu_intensive_task();
			return x+y+result;
		}

		my_sum_provider(tl::engine& e, uint16_t provider_id=1)
			: tl::provider<my_sum_provider>(e, provider_id) {
				define("sum", &my_sum_provider::sum );
				define("prod", &my_sum_provider::prod );
				get_engine().push_finalize_callback(this, [p=this]() { delete p; });
			}

	public:
		static my_sum_provider *create(tl::engine& e, uint16_t provider_id=1){
			return new my_sum_provider(e, provider_id);
		}
		~my_sum_provider() {
			get_engine().wait_for_finalize(); 
			get_engine().pop_finalize_callback(this);
		}
};
