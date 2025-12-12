#pragma once
#pragma warning(disable:4996)

// boost libray
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/tokenizer.hpp>


#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/assign.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#include <boost/geometry/index/rtree.hpp>

//BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)

namespace BoostGeometry = boost::geometry;