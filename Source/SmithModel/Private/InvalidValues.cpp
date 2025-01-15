// Fill out your copyright notice in the Description page of Project Settings.


#include "InvalidValues.h"
#include <limits>

// 無限大
const FVector InvalidValues::MapInvalidCoord_World = FVector( 
                                                        std::numeric_limits<double>::infinity(),
                                                        std::numeric_limits<double>::infinity(),
                                                        std::numeric_limits<double>::infinity()
                                                      );
