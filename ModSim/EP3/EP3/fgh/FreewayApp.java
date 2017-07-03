/*
Nome                        |   NUSP

Henrique Cerquinho          |  9793700
João Pedro Miguel de Moura  |  7971622
Tomás Paim                  |  7157602
Vítor Kei Taira Tamada      |  8516250
*/

import org.opensourcephysics.controls.*;
import org.opensourcephysics.frames.*;

public class FreewayApp extends AbstractSimulation
{
  double flowRate, density;
  Freeway freeway = new Freeway();
  DisplayFrame display = new DisplayFrame("Freeway");
  LatticeFrame spaceTime = new LatticeFrame("space", "time", "Space Time Diagram");
  
  public FreewayApp() { display.addDrawable(freeway); }

  public void initialize()
  {
    freeway.numberOfCars = control.getInt("Number of cars");
    freeway.roadLength = control.getInt("Road length");
    freeway.p = control.getDouble("Slow down probability");
    freeway.maximumVelocity = control.getInt("Maximum velocity");
    display.setPreferredMinMax(0, freeway.roadLength, -3, 4);
    freeway.initialize(spaceTime);
  }
  
  public void doStep()
  {
    //double velMean, velVar, velStdDev, gapsMean, gapsVar, gapsStdDev;
    freeway.step();
    
    // Calcula e imprime o flow rate e a densidade
    // Flow rate: velocidade média dos carros dividido pelo comprimento da pista
    // Densidade: número de carros dividido pelo comprimento da pista
    flowRate = 0;
    density = (double) freeway.numberOfCars;
    for(int i = 0; i < freeway.numberOfCars; i++)
      flowRate += (double) freeway.v[i];
    flowRate /= (double) freeway.numberOfCars;
    flowRate /= (double) freeway.roadLength;
    
    density /= (double) freeway.roadLength;
    /*
    velMean = freeway.velMean();
    velVar = freeway.velVar();
    velStdDev = freeway.velStdDev();
    
    gapsMean = freeway.gapsMean();
    gapsVar = freeway.gapsVar();
    gapsStdDev = freeway.gapsStdDev();
    
    System.out.println("==========\n" + velMean + " " + velVar + " " + velStdDev + " " + gapsMean + " " + gapsVar + " " + gapsStdDev);
    */
    System.out.println(flowRate + " " + density);
  }
  
  public void reset()
  {
    control.setValue("Number of cars", 10);
    control.setValue("Road length", 50);
    control.setValue("Slow down probability", 0.5);
    control.setValue("Maximum velocity", 2);
    control.setValue("Steps between plots", 1);
    enableStepsPerDisplay(true);
  }
  
  public void resetAverages()
  {
    freeway.flow = 0;
    freeway.steps = 0;
  }
  
  public static void main(String[] args)
  {
    Freeway model = new Freeway();
    SimulationControl control = SimulationControl.createApp(new FreewayApp());
    control.addButton("resetAverages", "resetAverages");
  }
}
