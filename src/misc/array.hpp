#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "data_types.hpp"

namespace util {
	template <typename Type, uint64 size>
	class Array {
	private:
		Type m_data[size];
	public:
		constexpr uint64 GetSize() const {
			return size;
		}

		Type& operator[](uint64 index) {
			return m_data[index];
		}
		const Type& operator[](uint64 index) const {
			return m_data[index];
		}

		Type* GetData() {
			return m_data;
		}
		const Type* GetData() const {
			return m_data;
		}
	};
}

#endif // !ARRAY_HPP


