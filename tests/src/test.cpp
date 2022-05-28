#include <boost/ut.hpp>
#include <pqrs/osx/iokit_iterator.hpp>
#include <pqrs/osx/iokit_types.hpp>

namespace {
pqrs::osx::iokit_iterator copy_constructor_test(void) {
  pqrs::osx::iokit_iterator result;
  return result;
}
} // namespace

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "iokit_iterator"_test = [] {
    {
      pqrs::osx::iokit_iterator iokit_iterator;
      expect(iokit_iterator.valid() == false);
    }

    {
      auto entry = IORegistryGetRootEntry(type_safe::get(pqrs::osx::iokit_mach_port::null));
      io_iterator_t it = IO_OBJECT_NULL;
      pqrs::osx::kern_return r = IORegistryEntryGetChildIterator(entry, kIOServicePlane, &it);
      expect(static_cast<bool>(r) == true);

      pqrs::osx::iokit_iterator iokit_iterator(it);
      expect(iokit_iterator.valid() == true);
    }
  };

  "iokit_iterator::copy_constructor_test"_test = [] {
    copy_constructor_test();
  };

  return 0;
}
