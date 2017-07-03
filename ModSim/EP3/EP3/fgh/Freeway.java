/*
Nome                        |   NUSP

Henrique Cerquinho          |  9793700
João Pedro Miguel de Moura  |  7971622
Tomás Paim                  |  7157602
Vítor Kei Taira Tamada      |  8516250
*/

import java.awt.Graphics;
import org.opensourcephysics.display.*;
import org.opensourcephysics.frames.*;
import org.opensourcephysics.display2d.*;
import org.opensourcephysics.controls.*;

public class Freeway implements Drawable
{
  public int[] v, x, xtemp, y; //y guarda a via do carro
  public LatticeFrame spaceTime;
  public double[] distribution;
  public int roadLength;
  public int numberOfCars;
  public int maximumVelocity;
  public double p; // probabilidade de reduzir a velocidade
  private CellLattice road;
  public double flow;
  public int steps, t;
  // número de iterações antes de scrollar o diagrama de espaço-tempo
  public int scrollTime = 100;
  
  public void initialize(LatticeFrame spaceTime)
  {
    this.spaceTime = spaceTime;
    x = new int[numberOfCars];
    xtemp = new int[numberOfCars]; // usado para permitir atualização em paralelo
    v = new int[numberOfCars];
    y = new int[numberOfCars]; //qual pista est?
    spaceTime.resizeLattice(roadLength, 100);
    //road de 2 via
    road = new CellLattice(roadLength, 2);
    //road de 1 via
    //road = new CellLattice(roadLength, 1);
    road.setIndexedColor(0, java.awt.Color.RED);
    road.setIndexedColor(1, java.awt.Color.GREEN);
    spaceTime.setIndexedColor(0, java.awt.Color.RED);
    spaceTime.setIndexedColor(1, java.awt.Color.GREEN);
    int d = roadLength/numberOfCars;
    x[0] = 0;
    v[0] = maximumVelocity;
    for (int i = 1; i < numberOfCars; i++)
    {
      x[i] = x[i-1] + d;
      if(Math.random() < 0.5) v[i] = 0;
      else                    v[i] = 1;
    }
    flow = 0;
    steps = 0;
    t = 0;
  }
  
  public void step()
  {
    for (int i = 0; i < numberOfCars; i++) xtemp[i] = x[i];
    
    boolean car_pos[][] = new boolean[roadLength][2];
    
    for (int i = 0; i < numberOfCars; i++) {
      car_pos[x[i]][y[i]] = true; 
    }
    
    for (int i = 0; i < numberOfCars; i++)
    {
      if (v[i] < maximumVelocity) v[i]++;
      //velocidades m?ximas diferentes est?o comentadas
      //if (i%2 == 0) {
      //  if (v[i] < maximumVelocity/2) v[i]++;
      //}
      //else if (v[i] < maximumVelocity) v[i]++;
      
      for (int j = 1; j <= v[i]; j++) {
        if(car_pos[(x[i]+j)%roadLength][y[i]] == true) {
          if(car_pos[(x[i]+1)%roadLength][(y[i]+1)%2] == false) {
            y[i] = (y[i]+1)%2;
            x[i] = (x[i]+1)%roadLength;
          }
          else { 
            v[i] = j-1;
            break;
          }
        }
      }
      if ((v[i] > 0) && (Math.random() < p)) v[i]--;
      
      x[i] = (xtemp[i] + v[i]) % roadLength;
      flow += v[i];
    }
    steps++;
    computeSpaceTimeDiagram();
  }
  
  public void computeSpaceTimeDiagram()
  {
    t++;
    if (t < scrollTime)
      for (int i = 0; i < numberOfCars; i++)
        spaceTime.setValue(x[i], t, 1);
    else
    {
      for (int y = 0; y < scrollTime - 1; y++)
        for (int i = 0; i < roadLength; i++)
          spaceTime.setValue(i, y, spaceTime.getValue(i, y+1));
      
      for (int i = 0; i < roadLength; i++)
        spaceTime.setValue(i, scrollTime - 1, 0);
      
      for (int i = 0; i < numberOfCars; i++)
        spaceTime.setValue (x[i], scrollTime - 1, 1);
    }
  }
  
  public void draw(DrawingPanel panel, Graphics g)
  {
    if (x == null) return;
    //modifica??es para acomodar 2 vias
    road.setBlock(0, 0, new byte[roadLength][2]);
    for (int i = 0; i < numberOfCars; i++) {
        road.setValue(x[i], y[i], (byte) 1);
    }
    
    road.draw(panel, g);
    g.drawString("Number of Steps = " + steps, 10, 20);
    g.drawString("Flow = " + ControlUtils.f3((double) flow / (roadLength * steps)), 10, 40);
    g.drawString("Density = " + ControlUtils.f3((double) numberOfCars / (roadLength)), 10, 60);
  }
}
