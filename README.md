# Fish And Plants

# INTRODUCTION
One of the most dangerous problem on earth is the amount of GHGs present in the air. 
Agricultural practices causes the 21% of the entire GHGs produced and causes the rising of temperature on earth (and more).
In this scenario, is obvious to see that our farming system is not efficient. 
Of course, we may limitate the meat consumption, or drive electric vehicles, but humanity (and not only) needs to feed, 
and our missions is reach the carbon neutrality in the entire food production with the least use of water and chemical substances. (2030 goals)

For many years, researchers have been trying to study different agricultural practices to reduce the GHGs.
A lot of solutions are good candidate to try to fix this problem. Our choices are the aeroponic greenhouse farming and the aquaponics greenhouse farming, 
because of their limit consumption of water and chemical products.

# Development
Our control system's brain is a Raspberry Pi 3B+ (but could be another type of mini-computer), that will run the code. Some temperature and moisture sensors will be used to acquire data and analyse the systems.
We have divided our job in three parts:
 1. Aeroponic greenhouse, where the code have to determine when irrigate and what percentage of N,P and K crops needs.
 2. Hydroponics greenhouse, where the code have to determine crop’s needs and provide to satisfy them. Then, we will want to use a robotic arm to try to harvest crops.
3. Aquaponics greenhouse, where code have to maintain water temperature constant, feed fishes and harvest crops.

## Aeroponic greenhouse
This code, collect data from a Raspberry's temperature and moisture sensors and evaluate when is the perfect time to irrigate, in order to reduce the water waste. It also evaluate the life conditions of plants and change the percentage of N, P and K in order to be efficient.


# Next milestones
We want to achieve point 1 of Development by April 2023.

