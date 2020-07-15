# Import our modules that we are using
import matplotlib.pyplot as plt
import numpy as np

# Battery in mAH
battery = 1100

# Average changes to the calendar per day. Appointments present at start of the day also count as one change per appointment.
changes_per_day = 20.0

# Durations in hours
duration_wake_up = 1.0 / 3600
duration_change = 9.0 / 3600

# Consumption in mA
consumption_low_power = 0.009  # 9uA
consumption_awake = 6

# Wakeup intervall in hours
wake_up_intervall = np.array(np.arange(0, 2.0, 0.025))

# The cycles per day
cycles_per_day = (24.0 - changes_per_day * duration_change) / (wake_up_intervall + duration_wake_up)
print(f"Cycles per day: {cycles_per_day}")

# The complete time for one cycle: suspended time + average update time per cycle + time for wakeup and suspend
total_cycle_time = 24.0 / cycles_per_day
print(f"Cycle time: {total_cycle_time * 3600}s")

# The average update time per cycle
avg_change_duration_per_cycle = changes_per_day * duration_change / cycles_per_day
print(f"Avg change time per cycle: {avg_change_duration_per_cycle * 3600}s")

# suspendtime in seconds * consumptions in suspended mode
abs_consumption_low_power = wake_up_intervall * consumption_low_power
print(f"Abs consumption while in sleep: {avg_change_duration_per_cycle * 3600}s")

# average changes per wakeupinterval * duration of a change * the consumption while updating
abs_consumption_changes = avg_change_duration_per_cycle * consumption_awake

# the consumption while waking up and going to sleep again wihtout any updates
abs_consumption_wakeup = duration_wake_up * consumption_awake

# (absolute consumption in low power + average absolut consumption for update per cycle + 
#  absolut consumption in wakup and suspend) / complete time for one cycle 
avg_consumption = (abs_consumption_low_power + abs_consumption_changes + abs_consumption_wakeup) / total_cycle_time
print(f"Average consumption: {avg_consumption}")

# (Battery capacity / average consumption) / hours of day
duration = (battery / avg_consumption) / 24

print("-----------------------------------------------------")
print("----------------     Result     ---------------------")
print("-----------------------------------------------------")
for pair in zip(wake_up_intervall * 60, duration):
    print(f"{round(pair[0], 1)} {round(pair[1], 2)}")
print("-----------------------------------------------------")

# Create the plot
plt.plot(wake_up_intervall * 60, duration)

plt.legend([f"Theoretical runtime with a {battery}mAh battery"])
plt.xlabel("Wakeup intervall (minutes)")
plt.ylabel("Runtime (days)")

# Show the plot
plt.show()