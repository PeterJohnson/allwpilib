// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.util.sendable2;

import edu.wpi.first.util.WPISerializable;

/**
 * Marker interface to indicate a class is serializable using Sendable serialization.
 *
 * <p>While this cannot be enforced by the interface, any class implementing this interface should
 * provide a public final static `sendable` member variable.
 */
public interface SendableSerializable extends WPISerializable {}