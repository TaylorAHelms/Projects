using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Obstacle Spawner
//Obstacles are spawned from 5 spawners placed at specific points down the track
//They come in 4 different sizes
//These obstacles are spawned at random intervals (either 1 or 2 seconds)
//and in 1 of 5 possible preset patterns, chosen by a random number generator

public class ObstacleSpawner : MonoBehaviour
{

    [SerializeField]
    private Transform FarLeft, MidLeft, Center, MidRight, FarRight;//obstacle spawn locations

    [SerializeField]
    private GameObject[] Obstacles;

    private GameObject spawnedObstacle1;
    private GameObject spawnedObstacle2;
    private GameObject spawnedObstacle3;
    private GameObject spawnedObstacle4;
    private GameObject spawnedObstacle5;

    private int randomSpawn;


    void Start()
    {
        StartCoroutine(SpawnObstacles());
    }

    IEnumerator SpawnObstacles()
    {
        while(true)
        {
            yield return new WaitForSeconds(Random.Range(1, 2));//determine the interval of time in between obstacle spawns

            randomSpawn = Random.Range(0, 5);

            if (randomSpawn == 0)//pattern 1: 5 small blocks evenly spaced
            {
                spawnedObstacle1 = Instantiate(Obstacles[0]);
                spawnedObstacle2 = Instantiate(Obstacles[0]);
                spawnedObstacle3 = Instantiate(Obstacles[0]);
                spawnedObstacle4 = Instantiate(Obstacles[0]);
                spawnedObstacle5 = Instantiate(Obstacles[0]);

                spawnedObstacle1.transform.position = FarLeft.position;
                spawnedObstacle2.transform.position = MidLeft.position;
                spawnedObstacle3.transform.position = Center.position;
                spawnedObstacle4.transform.position = MidRight.position;
                spawnedObstacle5.transform.position = FarRight.position;
            }
            else if (randomSpawn == 1)//pattern 2: 1 very large block in between 2 large blocks separated by small spaces
            {
                spawnedObstacle1 = Instantiate(Obstacles[2]);
                spawnedObstacle2 = Instantiate(Obstacles[3]);
                spawnedObstacle3 = Instantiate(Obstacles[2]);

                spawnedObstacle1.transform.position = FarLeft.position;
                spawnedObstacle2.transform.position = Center.position;
                spawnedObstacle3.transform.position = FarRight.position;
            }
            else if (randomSpawn == 2)//pattern 3: 1 block of each medium, small, and large from left to right
            {
                spawnedObstacle1 = Instantiate(Obstacles[2]);
                spawnedObstacle2 = Instantiate(Obstacles[1]);
                spawnedObstacle3 = Instantiate(Obstacles[3]);

                spawnedObstacle1.transform.position = FarLeft.position;
                spawnedObstacle2.transform.position = MidLeft.position;
                spawnedObstacle3.transform.position = Center.position;
            }
            else if (randomSpawn == 3)//pattern 4: 1 block of each large, small, and medium from left to right
            {
                spawnedObstacle1 = Instantiate(Obstacles[3]);
                spawnedObstacle2 = Instantiate(Obstacles[1]);
                spawnedObstacle3 = Instantiate(Obstacles[2]);

                spawnedObstacle1.transform.position = Center.position;
                spawnedObstacle2.transform.position = MidRight.position;
                spawnedObstacle3.transform.position = FarRight.position;
            }
            else if (randomSpawn == 4)//pattern 5: 1 medium block in between 2 large blocks separated by small spaces
            {
                spawnedObstacle1 = Instantiate(Obstacles[2]);
                spawnedObstacle2 = Instantiate(Obstacles[1]);
                spawnedObstacle3 = Instantiate(Obstacles[2]);

                spawnedObstacle1.transform.position = MidLeft.position;
                spawnedObstacle2.transform.position = Center.position;
                spawnedObstacle3.transform.position = MidRight.position;
            }
        }

    }

}//class







