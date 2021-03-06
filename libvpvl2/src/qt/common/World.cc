/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2010-2013  hkrn                                    */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAI project team nor the names of     */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include "vpvl2/qt/World.h"

namespace vpvl2
{
namespace qt
{

const Vector3 World::kAabbSize = Vector3(10000, 10000, 10000);
const Vector3 World::kDefaultGravity = Vector3(0, -9.8f, 0);

World::World()
    : m_dispatcher(0),
      m_broadphase(0),
      m_solver(0),
      m_world(0),
      m_motionFPS(0),
      m_maxSubSteps(0),
      m_fixedTimeStep(0)
{
    m_dispatcher = new btCollisionDispatcher(&m_config);
    m_broadphase = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, &m_config);
    setGravity(kDefaultGravity);
    setMotionFPS(Scene::defaultFPS());
}

World::~World()
{
    delete m_dispatcher;
    m_dispatcher = 0;
    delete m_broadphase;
    m_broadphase = 0;
    delete m_solver;
    m_solver = 0;
    delete m_world;
    m_world = 0;
}

const Vector3 World::gravity() const
{
    return m_world->getGravity();
}

void World::setGravity(const Vector3 &value)
{
    m_world->setGravity(value);
}

unsigned long World::randSeed() const
{
    return m_solver->getRandSeed();
}

void World::setRandSeed(unsigned long value)
{
    m_solver->setRandSeed(value);
}

void World::setMotionFPS(const Scalar &value)
{
    m_motionFPS = value;
    m_maxSubSteps = btMax(int(60 / m_motionFPS), 1);
    m_fixedTimeStep = 1 / value;
}

void World::addModel(vpvl2::IModel *value)
{
    value->joinWorld(m_world);
}

void World::removeModel(vpvl2::IModel *value)
{
    value->leaveWorld(m_world);
}

void World::addRigidBody(btRigidBody *value)
{
    m_world->addRigidBody(value);
}

void World::removeRigidBody(btRigidBody *value)
{
    m_world->removeRigidBody(value);
}

void World::stepSimulation(const Scalar &delta)
{
    m_world->stepSimulation(delta, m_maxSubSteps, m_fixedTimeStep);
}

btDiscreteDynamicsWorld *World::dynamicWorldRef() const
{
    return m_world;
}

} /* namespace qt */
} /* namespace vpvl2 */
