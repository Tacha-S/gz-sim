/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>
#include <thread>

#include "ignition/math/Stopwatch.hh"

using namespace ignition;

/////////////////////////////////////////////////
TEST(Stopwatch, Constructor)
{
  math::Stopwatch timeSys;

  EXPECT_FALSE(timeSys.Running());
  EXPECT_EQ(timeSys.StopTime(), timeSys.StartTime());
  EXPECT_EQ(ignition::math::clock::duration::zero(),
            timeSys.ElapsedRunTime());
  EXPECT_EQ(ignition::math::clock::duration::zero(),
            timeSys.ElapsedStopTime());
}

/////////////////////////////////////////////////
// Helper function that runs a few tests
void runTimer(math::Stopwatch &_time)
{
  // Start the timer
  EXPECT_TRUE(_time.Start());
  // The timer should be running.
  EXPECT_TRUE(_time.Running());
  // The start time should be greater than the stop time.
  EXPECT_GT(_time.StartTime(), _time.StopTime());
  // The elapsed stop time should still be zero.
  EXPECT_EQ(ignition::math::clock::duration::zero(),
            _time.ElapsedStopTime());

  // Wait for some time...
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // Now the elapsed time should be greater than or equal to the time slept.
  EXPECT_GE(_time.ElapsedRunTime(), std::chrono::milliseconds(1000));

  // Stop the timer.
  EXPECT_TRUE(_time.Stop());
  // The timer should not be running.
  EXPECT_FALSE(_time.Running());
  // The stop time should be greater than the start time.
  EXPECT_GT(_time.StopTime(), _time.StartTime());
  // The elapsed time should still be greater than the time slept.
  EXPECT_GE(_time.ElapsedRunTime(), std::chrono::milliseconds(1000));

  // Save the elapsed time.
  auto elapsedTime = _time.ElapsedRunTime();

  // The timer is now stopped, let's sleep some more.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // The elapsed stop time should not be greater than or equal to the time
  // slept.
  EXPECT_GE(_time.ElapsedStopTime(), std::chrono::milliseconds(1000));
  // The elapsed time should be the same.
  EXPECT_EQ(elapsedTime, _time.ElapsedRunTime());

  // Start the timer again.
  EXPECT_TRUE(_time.Start());
  // Store the elapsed stop time.
  auto elapsedStopTime = _time.ElapsedStopTime();
  // The timer should be running.
  EXPECT_TRUE(_time.Running());
  // Sleep for some time.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // The elapsed stop time should remain the same
  EXPECT_EQ(elapsedStopTime, _time.ElapsedStopTime());
  // The elapsed time should be greater than the previous elapsed time.
  EXPECT_GT(_time.ElapsedRunTime(), elapsedTime);
  // The elapsed time should be greater than or equal to the the previous
  // two sleep times.
  EXPECT_GE(_time.ElapsedRunTime(), std::chrono::milliseconds(2000));
}

/////////////////////////////////////////////////
TEST(Stopwatch, StartStopReset)
{
  math::Stopwatch timeSys;

  runTimer(timeSys);

  timeSys.Reset();

  EXPECT_FALSE(timeSys.Running());
  EXPECT_EQ(timeSys.StopTime(), timeSys.StartTime());
  EXPECT_EQ(ignition::math::clock::duration::zero(),
            timeSys.ElapsedRunTime());
  EXPECT_EQ(ignition::math::clock::duration::zero(),
            timeSys.ElapsedStopTime());

  runTimer(timeSys);

  EXPECT_TRUE(timeSys.Running());
  timeSys.Start(true);
  EXPECT_TRUE(timeSys.Running());
  EXPECT_LT(timeSys.StopTime(), timeSys.StartTime());
  EXPECT_NE(ignition::math::clock::duration::zero(),
            timeSys.ElapsedRunTime());
  EXPECT_EQ(ignition::math::clock::duration::zero(),
            timeSys.ElapsedStopTime());
}
