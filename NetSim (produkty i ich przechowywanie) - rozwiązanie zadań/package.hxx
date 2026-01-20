#ifndef PACKAGE_HXX
#define PACKAGE_HXX

#include <set>
#include "types.hxx"

class Package {
 public:
	Package();
	Package(ElementID id);
	Package(Package && other) noexcept;
	Package & operator=(Package && other) noexcept;
	~Package();

	ElementID get_id()const {return id_;}

 private:
	ElementID id_;
	static std::set<ElementID> free_ids;
	static std::set<ElementID> assigned_ids;
};

#endif // PACKAGE_HXX
