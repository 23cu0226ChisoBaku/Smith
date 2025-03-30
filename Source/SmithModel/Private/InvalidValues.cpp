// Fill out your copyright notice in the Description page of Project Settings.


#include "InvalidValues.h"

#include <limits>

// 無限大
const FVector InvalidValues::MAP_COORD_WORLD_INVALID = FVector( 
                                                        std::numeric_limits<double>::infinity(),
                                                        std::numeric_limits<double>::infinity(),
                                                        std::numeric_limits<double>::infinity()
                                                      );

const uint8 InvalidValues::MAP_COORD_INVALID = 255u;
