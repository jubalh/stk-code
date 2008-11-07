//  $Id: flyable.hpp 1284 2007-11-08 12:31:54Z hikerstk $
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2007 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_FLYABLE_H
#define HEADER_FLYABLE_H

#include "moveable.hpp"
#include "moving_physics.hpp"
#include "kart.hpp"
#include "terrain_info.hpp"

class FlyableInfo;

class Flyable : public Moveable, public TerrainInfo
{
public:
private:
    bool              m_has_hit_something;
    /** This flag is used to avoid that a rocket explodes mode than once.
     *  It can happen that more than one collision between a rocket and
     *  a track or kart is reported by the physics.                        */   
    bool              m_exploded;
    /** If this flag is set, the Z velocity of the kart will not be
     *  adjusted in case that the objects is too high or too low above the 
     *  terrain. Otherwise gravity will not work correctly on this object. */
    bool              m_adjust_z_velocity;

protected:
    Kart*             m_owner;              // the kart which released this flyable
    btCollisionShape *m_shape;
    float             m_max_height;
    float             m_min_height;
    float             m_average_height;     // average of m_{min,ax}_height
    float             m_force_updown;
    float             m_speed;
    float             m_mass;
    btVector3         m_extend;
    // The flyable class stores the values for each flyable type, e.g. 
    // speed, min_height, max_height. These variables must be static,
    // so we need arrays of these variables to have different values
    // for bowling balls, missiles, ...
    static float      m_st_speed[POWERUP_MAX];         // Speed of the projectile
    static ssgEntity* m_st_model[POWERUP_MAX];         // 3d model
    static float      m_st_min_height[POWERUP_MAX];    // min height above track
    static float      m_st_max_height[POWERUP_MAX];    // max height above track
    static float      m_st_force_updown[POWERUP_MAX];  // force pushing up/down 
    static btVector3  m_st_extend[POWERUP_MAX];        // size of the model

    /** Returns information on what is the closest kart and at what
        distance it is. All 3 parameters first are of type 'out'.
        'inFrontOf' can be set if you wish to know the closest
        kart in front of some karts (will ignore those behind).
        Useful e.g. for throwing projectiles in front only.
     */
    void              getClosestKart(const Kart **minKart, float *minDistSquared, 
                                     btVector3 *minDelta, const Kart* inFrontOf=NULL) const;
    /** init bullet for moving objects like projectiles */
    void              createPhysics(float y_offset, 
                                    const btVector3 velocity,
                                    btCollisionShape *shape, const bool gravity=false,
                                    const bool rotates=false, const btTransform* customDirection=NULL);
public:

                 Flyable     (Kart* kart, PowerupType type, float mass=1.0f);
    virtual     ~Flyable     ();
    /** Enables/disables adjusting ov velocity depending on height above 
     *  terrain. Missiles can 'follow the terrain' with this adjustment,
     *  but gravity will basically be disabled.                          */
    void         setAdjustZVelocity(bool f) { m_adjust_z_velocity = f; }
    static void  init        (const lisp::Lisp* lisp, ssgEntity *model, 
                              PowerupType type);
    virtual void update      (float);
    void         updateFromServer(const FlyableInfo &f, float dt);

    virtual void hitTrack    () {};
    void         explode     (Kart* kart, MovingPhysics* moving_physics=NULL);
    bool         hasHit      ()               { return m_has_hit_something; }
    void         reset       () { Moveable::reset(); }
};   // Flyable

#endif
