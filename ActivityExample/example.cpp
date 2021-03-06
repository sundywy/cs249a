#include "Activity.h"
#include "ActivityReactor.h"

int main(int argc, char* argv[]) {
    
    Activity::Manager::Ptr manager = activityManagerInstance();

    Activity::Ptr prod1 = manager->activityNew("producer 1");
    Activity::Ptr prod2 = manager->activityNew("producer 2");
    Activity::Ptr prod3 = manager->activityNew("producer 3");

    Activity::Ptr consumer1 = manager->activityNew("consumer 1");
    
    //producer 1 produces at rate of once every 2 seconds
    prod1->lastNotifieeIs(new ProducerActivityReactor(manager,
						      prod1.ptr(), 2)); 
    //producer 2 produces at rate of once every 10 seconds
    prod2->lastNotifieeIs(new ProducerActivityReactor(manager,
						      prod2.ptr(), 10));
    //producer 3 produces at rate of once every 2 seconds
    prod3->lastNotifieeIs(new ProducerActivityReactor(manager,
						      prod3.ptr(), 2));
    //consumer 1 consumes at rate of once every second.
    consumer1->lastNotifieeIs(new ConsumerActivityReactor(manager,
							  consumer1.ptr(),
							  1));


    //schedule producer and consumer to run for the first time
    prod1->statusIs(Activity::nextTimeScheduled);
    
    //producer 2 starts at time 2.0
    prod2->nextTimeIs(2.0);
    prod2->statusIs(Activity::nextTimeScheduled);
    
    //producer 3 starts at time 3.0
    prod3->nextTimeIs(3.0);
    prod3->statusIs(Activity::nextTimeScheduled);
    
    consumer1->statusIs(Activity::nextTimeScheduled);

    //set the time to 20.0
    manager->nowIs(20.0);
    
    return 0;
}

