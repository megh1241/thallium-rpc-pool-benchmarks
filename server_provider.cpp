#include <iostream>
#include <thallium.hpp>
#include <thallium/serialization/stl/string.hpp>

namespace tl = thallium;

class my_sum_provider : public tl::provider<my_sum_provider> {
	private:
		void prod(const tl::request& req, int x, int y) {
			req.respond(x*y);
		}
		int sum(int x, int y) {
			return x+y;
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
