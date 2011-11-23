/*
 * ForwardActivityReactor.h
 *
 *  Created on: Nov 22, 2011
 *      Author: per
 */

#ifndef FORWARDACTIVITYREACTOR_H_
#define FORWARDACTIVITYREACTOR_H_

#include "Activity.h"
#include "Utils.h"
#include "Fleet.h"
#include "Segment.h"

namespace Shipping{
class Shipment;
class ForwardActivityReactor : public Fwk::Activity::Notifiee {
public:
    ForwardActivityReactor(Fwk::Ptr<Fwk::Activity::Manager>,
                           Fwk::Activity*,
                           Fwk::Ptr<Fleet const>,
                           Fwk::Ptr<Segment> ,
                           Fwk::Ptr<Shipment>);
    virtual void onStatus();
protected:
       Fwk::Activity::Ptr activity_;
       Fwk::Ptr<Fwk::Activity::Manager> manager_;
       Fwk::Ptr<Fleet const> fleet_;
       Fwk::Ptr<Shipment> shipment_;
       Fwk::Ptr<Segment> segment_;
       PackageCount activePackages_;
       void removeActivePackagesFromSegment();
       void addActivePackagesToSegment();
       void forwardActivity();
       Fleet::Vehicle segTypeToFleetVehicle(Segment::SegmentType);

};
}//end namespace Shipping

#endif /* FORWARDACTIVITYREACTOR_H_ */
