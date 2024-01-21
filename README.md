# FreeRTOS tests

## Overview
The exercises focus on advanced FreeRTOS concepts using the Arduino IDE and Raspberry PI Pico boards.

## Task 1: LED Blink Synchronization

### Objective
- **Challenge**: Address the issue of unsynchronized LED blinking, a common problem in concurrent programming.
- **Purpose**: Demonstrates understanding of synchronization in a real-time operating system (RTOS). This is essential for tasks that require precise timing and coordination, common in embedded systems.

### Description
- **Implementation**: Utilized FreeRTOS synchronization mechanisms to control the timing of the LED blinks. This approach ensures that the LED blinks consistently.
  
## Task 2: Queue-based LED Control

### Objective
- **Challenge**: Implement queue-based communication to control an LED, showcasing inter-task message passing.
- **Purpose**: Illustrates the use of queues for task communication in RTOS, a crucial feature for complex systems where tasks need to exchange information or signals reliably.

### Description
- **Implementation**: Developed a program where the LED blinks on and off every second, controlled by queue messages. The reception of specific values (100 and 200) in the queue dictates the LED's state, demonstrating effective inter-task communication.

## Task 3: Rate Monotonic Scheduler Design

### Objective
- **Challenge**: Design a scheduler for tasks with differing execution times and periods, testing knowledge of scheduling algorithms.
- **Purpose**: Highlights skills in creating efficient, real-time scheduling, critical for ensuring timely task execution in systems with multiple time-sensitive tasks.

### Description
- **Implementation**: Created a Rate Monotonic Scheduler accommodating two tasks with specified execution times and periods. The solution showcases real-time task management and prioritization in FreeRTOS.
