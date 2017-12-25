//
//  cam.hpp
//  cylinder_opengl
//
//  Created by Wolfgang Schulz on 25.12.17.
//  Copyright © 2017 EkwoTECH GmbH. All rights reserved.
//

#ifndef cam_hpp
#define cam_hpp

#include <stdio.h>
#include "dreidee.hpp"

// ortogonal view

// perspective view
struct camera {
  dreidee::vec3 eye;
  dreidee::vec3 center;
  dreidee::vec3 up;
  int t;
};

#endif /* cam_hpp */
