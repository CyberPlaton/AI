# AI
Exploring Artificial Intelligence based.  
Note: The solution is not meant to show optimized code and serves only as a testbed.

## References: 
"Behavior Trees in Robotics and Al. An Introduction", Michele Colledanchise.  
"AI for Games", Ian Millington.

## Tested Behavior Tree 1)
![alt text](https://github.com/CyberPlaton/AI/blob/master/BT_0.1.png)

## Tested Behavior Tree 2)
![alt text](https://github.com/CyberPlaton/AI/blob/master/BT_0.2.png)

## Tested Behavior Tree 3)
![alt text](https://github.com/CyberPlaton/AI/blob/master/BT_0.3.png)

# Heightmap generation compared to Perlin Noise  
## Heightmap 1)
![alt text](https://github.com/CyberPlaton/AI/blob/master/HeightMaps_1.png)
## Heightmap 2)
![alt text](https://github.com/CyberPlaton/AI/blob/master/HeightMaps_2.png)

# Random map generation from perlin height and moisture noise maps.
## Legend:  
### Green: Temeperate
### Light Yellow: Sand
### Darker Yellow: Savannah
### Dark Green: Tundra
### Violet: Jungle
### Brown: Mountains
### Light Blue: Shallow Water
### Dark Blue: Deep Water
### Black Dots: Forest

## Maps:  
### Left Up: Height Map.
### Right Up: Moisture Map.
### Left Down: Biome Map, created with height and moisture maps.
### Right Down: Final Map, created from previous maps.  
![alt text](https://github.com/CyberPlaton/AI/blob/master/WorldGeneration_1.png)
![alt text](https://github.com/CyberPlaton/AI/blob/master/WorldGeneration_2.png)

## Nation Generation:  
### Left Up: Final World Map from previous steps.  
### Right Up: Culture Archetype Distribution Map (where we have 12 archetypes).  
### Left Down: "Race" Distribution Map (where we have 8 races, e.g. High Elfes, Orcs).  
### Right Down: Final World Map with Nation placement and their simulation after x steps.  
#### In a game these steps would be required on world map generation, after which the player
#### can choose a nation to play as. The simulation can be easily altered with new nation and race placements or even new map generation data.  
![alt text](https://github.com/CyberPlaton/AI/blob/master/WorldGeneration_3.png)
![alt text](https://github.com/CyberPlaton/AI/blob/master/WorldGeneration_4.png)
