/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.simulation;

import edu.wpi.first.hal.sim.NotifyCallback;
import edu.wpi.first.hal.sim.mockdata.DutyCycleDataJNI;
import edu.wpi.first.wpilibj.DutyCycle;
import java.util.NoSuchElementException;

public class DutyCycleSim {
  private final int m_index;

  public DutyCycleSim(DutyCycle dutyCycle) {
    m_index = dutyCycle.getFPGAIndex();
  }

  public DutyCycleSim(int index) {
    m_index = index;
  }

  public static DutyCycleSim createForChannel(int channel) {
    int index = DutyCycleDataJNI.findDutyCycleForChannel(channel);
    if (index < 0) {
      throw new NoSuchElementException("no duty cycle found for channel " + channel);
    }
    return new DutyCycleSim(index);
  }

  public static DutyCycleSim createForIndex(int index) {
    return new DutyCycleSim(index);
  }

  public CallbackStore registerInitializedCallback(NotifyCallback callback, boolean initialNotify) {
    int uid = DutyCycleDataJNI.registerInitializedCallback(m_index, callback, initialNotify);
    return new CallbackStore(m_index, uid, DutyCycleDataJNI::cancelInitializedCallback);
  }
  public boolean getInitialized() {
    return DutyCycleDataJNI.getInitialized(m_index);
  }
  public void setInitialized(boolean initialized) {
    DutyCycleDataJNI.setInitialized(m_index, initialized);
  }

  public CallbackStore registerFrequencyCallback(NotifyCallback callback, boolean initialNotify) {
    int uid = DutyCycleDataJNI.registerFrequencyCallback(m_index, callback, initialNotify);
    return new CallbackStore(m_index, uid, DutyCycleDataJNI::cancelFrequencyCallback);
  }
  public int getFrequency() {
    return DutyCycleDataJNI.getFrequency(m_index);
  }
  public void setFrequency(int frequency) {
    DutyCycleDataJNI.setFrequency(m_index, frequency);
  }

  public CallbackStore registerOutputCallback(NotifyCallback callback, boolean initialNotify) {
    int uid = DutyCycleDataJNI.registerOutputCallback(m_index, callback, initialNotify);
    return new CallbackStore(m_index, uid, DutyCycleDataJNI::cancelOutputCallback);
  }
  public double getOutput() {
    return DutyCycleDataJNI.getOutput(m_index);
  }
  public void setOutput(double output) {
    DutyCycleDataJNI.setOutput(m_index, output);
  }
}
