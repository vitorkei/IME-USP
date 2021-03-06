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
  public int[] v, x, xtemp;
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
    spaceTime.resizeLattice(roadLength, 100);
    road = new CellLattice(roadLength, 1);
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
    for (int i = 0; i < numberOfCars; i++)
    {
      if (v[i] < maximumVelocity) v[i]++;
      
      // distância entre os carros
      int d = xtemp[(i+1) % numberOfCars] - xtemp[i];
      
      if (d <= 0) d += roadLength;
      if (v[i] >= d) v[i] = d-1;
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
  
  // Funções que retornam a média variância e
  //   desvio padrão da velocidade dos carros.
  public double velMean()
  {
    double mean = 0;
    for (int i = 0; i < numberOfCars; i++)
      mean += (double) v[i];
    mean /= (double) numberOfCars;
    return mean;
  }
  public double velVar()
  {
    double var = 0;
    double mean = velMean();
    double aux = 0;
    for (int i = 0; i < numberOfCars; i++)
    {
      aux = v[i] - mean;
      var += aux * aux;
    }
    var /= (numberOfCars - 1);
    return var;
  }
  public double velStdDev() { return Math.sqrt(velVar()); }
  
  // Distribuição de espaços - distâncias entre um carro
  //   e o carro logo a frente, média, variância e desvio padrão
  public int[] gaps()
  {
    int aux;
    int[] gaps = new int[numberOfCars - 1];
    for (int i = 0; i < gaps.length; i++)
      gaps[i] = Math.abs(x[i+1] - x[i]);
    
    return gaps;
  }
  public double gapsMean()
  {
    int[] gaps = gaps();
    double mean = 0;
    for (int i = 0; i < gaps.length; i++)
      mean += (double) gaps[i];
    mean /= (double) gaps.length;
    return mean;
  }
  public double gapsVar()
  {
    int[] gaps = gaps();
    double mean = gapsMean();
    double var = 0;
    double aux = 0;
    for (int i = 0; i < gaps.length; i++)
    {
      aux = gaps[i] - mean;
      var += aux * aux;
    }
    var /= (gaps.length - 1);
    return var;
  }
  public double gapsStdDev() { return Math.sqrt(gapsVar()); }
  
  public void draw(DrawingPanel panel, Graphics g)
  {
    if (x == null) return;
    
    road.setBlock(0, 0, new byte[roadLength][1]);
    for (int i = 0; i < numberOfCars; i++)
      road.setValue(x[i], 0, (byte) 1);
    
    road.draw(panel, g);
    g.drawString("Number of Steps = " + steps, 10, 20);
    g.drawString("Flow = " + ControlUtils.f3((double) flow / (roadLength * steps)), 10, 40);
    g.drawString("Density = " + ControlUtils.f3((double) numberOfCars / (roadLength)), 10, 60);
  }
}
