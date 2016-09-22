#include <tuple>
#include <utility>
#include <cassert>

template<typename T, typename... Names>
struct static_map
{
private:
  	template<typename Name>
	struct element
	{
  		using name = Name;
  		explicit element(T v) : value(v) {}
		T value;  
	};
public:	  
  	template<typename... Args>
  	static_map(Args&&... args) 
      : m_elements(std::make_tuple(element<Names>(std::forward<Args>(args))...))
    {
      	static_assert(sizeof...(Names) == sizeof...(Args), "what are you doing?");
    }
  
	template<typename Name>
    decltype(auto) get() const
    {
    	return std::get<element<Name>>(m_elements).value;
    }
  
  	template<typename Name>
    void set(const T& v)
    {
      	std::get<element<Name>>(m_elements).value = v;
    }
  
  	template<typename Name>
    void set(T&& v)
    {
      	std::get<element<Name>>(m_elements).value = std::move_if_noexcept(v);
    }
private:
  	std::tuple<element<Names>...> m_elements;
};

int main()
{
  	using point2d = static_map<double, struct X, struct Y>;
  	using point3d = static_map<double, struct X, struct Y, struct Z>;
  
  	using double_array = double[5];
  	using point5d = static_map<double, struct A, struct B, struct C, struct D, struct E>;

  	double_array da;
  	point5d p5d(1,2,3,4,5);
  
  	static_assert(sizeof(double_array) == sizeof(point5d), "size check by type");
  	static_assert(sizeof(da) == sizeof(p5d), "size check by value");
  	
  	point2d p2d(1, 2);
  	point3d p3d(3, 5, 10);
  
  	assert(p2d.get<X>() == 1);
  	assert(p2d.get<Y>() == 2);
  
  	assert(p3d.get<X>() == 3);
  	assert(p3d.get<Y>() == 5);
  	assert(p3d.get<Z>() == 10);
  
  	p2d.set<X>(33);
  	p3d.set<Y>(7);
  
  	assert(p2d.get<X>() == 33);
  	assert(p3d.get<Y>() == 7);
  
  	return 0;
}
    