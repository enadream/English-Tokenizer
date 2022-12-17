#ifndef ARRAY_HPP
#define ARRAY_HPP

namespace util {
	template <typename Type, size_t size>
	class Array {
	private:
		Type m_data[size];
	public:
		constexpr size_t GetSize() const {
			return size;
		}

		Type& operator[](size_t index) {
			return m_data[index];
		}
		const Type& operator[](size_t index) const {
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


