#include <boost/ut.hpp>
#include <pqrs/osx/iokit_iterator.hpp>
#include <pqrs/osx/iokit_types.hpp>
#include <utility>

namespace {
pqrs::osx::iokit_iterator copy_constructor_test() {
  pqrs::osx::iokit_iterator result;
  return result;
}

io_iterator_t make_child_iterator() {
  auto entry = pqrs::osx::adopt_iokit_object_ptr(IORegistryGetRootEntry(type_safe::get(pqrs::osx::iokit_mach_port::null)));
  boost::ut::expect(static_cast<bool>(entry) == true);

  io_iterator_t it = IO_OBJECT_NULL;
  pqrs::osx::kern_return r = IORegistryEntryGetChildIterator(*entry, kIOServicePlane, &it);
  boost::ut::expect(static_cast<bool>(r) == true);

  return it;
}
} // namespace

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "default constructor"_test = [] {
    pqrs::osx::iokit_iterator iokit_iterator;

    expect(iokit_iterator.get().get() == IO_OBJECT_NULL);
    expect(iokit_iterator.valid() == false);
    expect(static_cast<bool>(iokit_iterator) == false);
    expect(iokit_iterator.next().get() == IO_OBJECT_NULL);
    iokit_iterator.reset();
  };

  "copy constructor"_test = [] {
    copy_constructor_test();
  };

  "child iterator"_test = [] {
    auto iokit_iterator = pqrs::osx::adopt_iokit_iterator(make_child_iterator());
    expect(iokit_iterator.valid() == true);
    expect(static_cast<bool>(iokit_iterator) == true);

    auto first = iokit_iterator.next();
    expect(static_cast<bool>(first) == true);
    expect(first.class_name().has_value() == true);

    iokit_iterator.reset();
    expect(iokit_iterator.valid() == true);

    auto first_after_reset = iokit_iterator.next();
    expect(static_cast<bool>(first_after_reset) == true);
    expect(first_after_reset.get() == first.get());
  };

  "constructor from iokit_object_ptr"_test = [] {
    {
      auto iterator_object = pqrs::osx::adopt_iokit_object_ptr(make_child_iterator());
      pqrs::osx::iokit_iterator iokit_iterator(iterator_object);
      expect(iokit_iterator.valid() == true);
    }
  };

  "constructor from moved iokit_object_ptr"_test = [] {
    auto iterator_object = pqrs::osx::adopt_iokit_object_ptr(make_child_iterator());
    pqrs::osx::iokit_iterator iokit_iterator(std::move(iterator_object));
    expect(iokit_iterator.valid() == true);
    expect(static_cast<bool>(iterator_object) == false);
  };

  "retain count"_test = [] {
    {
      auto it = make_child_iterator();
      auto retain_count = IOObjectGetUserRetainCount(it);
      expect(retain_count > 0U);

      auto iokit_iterator = pqrs::osx::adopt_iokit_iterator(it);
      expect(iokit_iterator.get().get() == it);
      expect(iokit_iterator.get().user_retain_count() == retain_count);
    }

    {
      auto iterator_object = pqrs::osx::adopt_iokit_object_ptr(make_child_iterator());
      auto retain_count = iterator_object.user_retain_count();
      expect(retain_count > 0U);

      pqrs::osx::iokit_iterator iokit_iterator(iterator_object);
      expect(iterator_object.user_retain_count() == retain_count + 1);
      expect(iokit_iterator.get().user_retain_count() == retain_count + 1);
    }

    {
      auto iterator_object = pqrs::osx::adopt_iokit_object_ptr(make_child_iterator());
      auto retain_count = iterator_object.user_retain_count();
      expect(retain_count > 0U);

      pqrs::osx::iokit_iterator iokit_iterator(std::move(iterator_object));
      expect(iokit_iterator.get().user_retain_count() == retain_count);
      expect(static_cast<bool>(iterator_object) == false);
    }

    {
      auto iokit_iterator = pqrs::osx::adopt_iokit_iterator(make_child_iterator());
      auto first = iokit_iterator.next();
      expect(static_cast<bool>(first) == true);
      expect(first.user_retain_count() == 1U);
    }
  };

  return 0;
}
