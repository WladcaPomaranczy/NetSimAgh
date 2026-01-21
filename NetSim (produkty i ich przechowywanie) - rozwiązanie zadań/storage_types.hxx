#ifndef STORAGE_TYPES_HXX
#define STORAGE_TYPES_HXX

#include <list>

#include "package.hxx"

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package && package);
    virtual bool empty() const;
    virtual const_iterator begin() const;
    virtual const_iterator end() const;
    virtual std::size_t size() const;
    virtual ~IPackageStockpile();
};

class IPackageQueue:public IPackageStockpile {
    public:
    virtual Package pop();
    virtual const PackageQueueType get_queue_type() const;
};

class PackageQueue:IPackageQueue {

    public:
    PackageQueue(PackageQueueType package_queue_type): package_queue_type_(package_queue_type) {};
    PackageQueue(PackageQueueType package_queue_type, std::list<Package> &&package_queue): package_queue_type_(package_queue_type), package_queue_(std::move(package_queue)) {};
    void push(Package && package) override{package_queue_.push_back(std::move(package));}
    bool empty() const override {return package_queue_.empty();}
    std::size_t size() const override {return package_queue_.size();}
    const_iterator begin() const override {return package_queue_.begin();}
    const_iterator end() const override {return package_queue_.end();}

    Package pop() override;
    const PackageQueueType get_queue_type() const override {return package_queue_type_;}


    private:
    std::list<Package> package_queue_;
    PackageQueueType package_queue_type_;
};

#endif // STORAGE_TYPES_HXX
