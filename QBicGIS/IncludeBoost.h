#pragma once

// boost libray
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/any.hpp>
#include <boost/tokenizer.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/assign.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#include <boost/geometry/index/rtree.hpp>

BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)
namespace BoostGeometry = boost::geometry;
typedef BoostGeometry::model::point  <double, 2,     BoostGeometry::cs::cartesian> BoostPoint;
typedef BoostGeometry::model::box    <BoostPoint>    BoostEnvelope;
typedef BoostGeometry::model::polygon<BoostPoint, true, false> BoostPolygon;
typedef BoostGeometry::model::ring   <BoostPoint>       BoostRing;

typedef BoostGeometry::model::linestring<BoostPoint> BoostPolyline;

#ifndef BOOST_GEOMETRY_INDEX_DETAIL_EXPERIMENTAL
  #define BOOST_GEOMETRY_INDEX_DETAIL_EXPERIMENTAL
#endif
