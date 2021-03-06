#include "VirtualTimeActivityManager.h"
#include "RealTimeActivityManager.h"
#include "RealTimeActivityReactor.h"
#include "Output.h"
#include "Exception.h"
#include <sstream>
#include <math.h>

using namespace Shipping;
long unsigned int VirtualTimeActivityManager::idx = 0;

Fwk::Activity::Ptr VirtualTimeActivityManager::activityNew(const string &_name){
    Fwk::Activity::Ptr activity = activities_[_name];
    if (activity != NULL) {
        std::cerr << "Activity already exists!" << std::endl;
        throw(Fwk::NameInUseException(
            "VirtualTimeActivityManager::activityNew"));
    }
    activity = new Fwk::Activity(_name, this);
    if (!activity) {
        std::cerr << "VirtualTimeActivityManager::activityNew new() failed" << 
            std::endl;
        throw(Fwk::MemoryException("VirtualTimeActivityManager::activityNew"));
    }
    activities_[_name] = activity;
    return activity;
}

Fwk::Activity::Ptr
VirtualTimeActivityManager::activity(const string &_name) const {
    std::map<std::string, Fwk::Activity::Ptr>::const_iterator it =
            activities_.find(_name);
    if(it != activities_.end() ) {
        return (*it).second;
    }
    std::cerr << "Activity " << _name << " not found" << std::endl;
    throw(Fwk::EntityNotFoundException("VirtualTimeActivityManager::activity"));
}

void VirtualTimeActivityManager::activityDel(const string &_name) {
    activities_.erase(_name);
}

void VirtualTimeActivityManager::lastActivityIs(Fwk::Activity::Ptr _activity) {
    FWK_DEBUG("VirtualTimeActivityManager::lastActivityIs " <<
        _activity->name());
    scheduledActivities_.push(_activity);
    SIM(_activity->name() << " is queued for execution at: " <<
        _activity->nextTime().value());
    ++idx;

    std::stringstream ss;
    ss << "RealTimeActivity" << VirtualTimeActivityManager::activityIndex();
    Fwk::Activity::Ptr realTimeActivity =realTimeActMgr_->activityNew(ss.str());
    Fwk::Time nextTime =
            _activity->nextTime().value() * realTimeActMgr_->scale().value();
    realTimeActivity->nextTimeIs(_activity->nextTime().value() *
        realTimeActMgr_->scale().value());
    realTimeActivity->lastNotifieeIs(
            new RealTimeActivityReactor(
                ss.str() + std::string("Reactor"),
                realTimeActivity.ptr(),
                this,
                realTimeActMgr_,
                _activity->nextTime()
            )
    );
    realTimeActivity->statusIs(Fwk::Activity::nextTimeScheduled);
}

void VirtualTimeActivityManager::nowIs(Fwk::Time t) {
    FWK_DEBUG("VirtualTimeActivityManager::nowIs " << t.value());
    while (!scheduledActivities_.empty()) {
        Fwk::Activity::Ptr nextToRun = scheduledActivities_.top();
        if (nextToRun->nextTime() > t) {
            break;
        }
        Fwk::Time diff = Fwk::Time(nextToRun->nextTime().value() -now_.value());
        now_ = nextToRun->nextTime();

        unsigned int hours = floor(now_.value());
        unsigned int min = (now_.value() - hours) * 60;
        SIM(std::endl << "[" << hours << ":" << min << "] " << nextToRun->name());
        scheduledActivities_.pop();
        nextToRun->statusIs(Fwk::Activity::executing);
        nextToRun->statusIs(Fwk::Activity::free);
    }
    now_ = t;
}

void VirtualTimeActivityManager::simluationEndIs(Fwk::Time _time) {
    FWK_DEBUG("VirtualTimeActivityManagaer::simulationEndIs " << _time.value());
    if (_time.value() < now_.value()){
        //error bla bla. Time can't be less than the current value
    }
    simulationEnd_ = _time;
    RealTimeActivityManager::TimeScale scale = realTimeActMgr_->scale();
    Fwk::Time realTimeSimulationEnd =
            Fwk::Time(simulationEnd_.value() * scale.value());
    realTimeActMgr_->nowIs(realTimeSimulationEnd);
};

VirtualTimeActivityManager::VirtualTimeActivityManager(
        const std::string &_name) :
        Fwk::Activity::Manager(_name), now_(0.0), simulationEnd_(0.0) {}

Shipping::VirtualTimeActivityManager::Ptr
Shipping::VirtualTimeActivityManager::VirtualTimeActivityManagerNew(
        const std::string &_name) {
    VirtualTimeActivityManager::Ptr p = new VirtualTimeActivityManager(_name);
    if (!p) {
        std::cerr << "VirtualTimeActivityManagerNew new() failed" << std::endl;
        throw(Fwk::MemoryException("VirtualTimeActivityManagerNew"));
    }
    return p;
}

void Shipping::VirtualTimeActivityManager::realTimeActivityManagerIs(
        Fwk::Ptr<RealTimeActivityManager> _realTimeActMgr) {
    realTimeActMgr_ = _realTimeActMgr;
}






