#include <iostream>


struct IntArray {
  void add(int i);
  int get(size_t id) const noexcept;
  size_t size() const noexcept;
  int last() const noexcept;
  IntArray(int i);
  ~IntArray();
  int * a;
  size_t k;
}; 


int main()
{
  int next = 0;
  std::cin >> next;
  try {
    IntArray a(next);
    while (std::cin >> next)
    {
      a.add(next);
    }
    if (std::cin.fail() && !std::cin.eof())
    {
      return 1;
    }
    size_t count = 0;
    for (size_t i = 0; i < a.size(); ++i)
    {
      if (a.last() != 0 && a.get(i) % a.last() == 0)
      {
        count++;
      }
    }
    std::cout << count << "\n";
  }
  catch (const std::bad_alloc&) {
    return 2;
  }
  return 0;
}

IntArray::~IntArray() {
  delete[] a;
}

IntArray::IntArray(int i) :
  a(new int[1]),
  k(1)
{
  a[0]  = i;
}

int IntArray::get(size_t id) const noexcept
{
  return a[id];
}

size_t IntArray::size() const noexcept
{
  return k;
}

int IntArray::last() const noexcept
{
  return get(size() - 1);
}

void IntArray::add(int i)
{
  int * tmp = new int[size() + 1];
  for (size_t i = 0; i < size(); ++i)
  {
    tmp[i] = get(i);
  }
  tmp[size()] = i;
  delete[] a;
  a = tmp;
  ++k;
}

