## Strategy description

The strategy coefficient is defined as:

<math xmlns="http://www.w3.org/1998/Math/MathML" display="block">
  <mtable displaystyle="true" columnalign="right" columnspacing="" rowspacing="3pt">
    <mtr>
      <mtd>
        <mi>S</mi>
        <mo>=</mo>
        <mfrac>
          <mrow>
            <mrow data-mjx-texclass="ORD">
              <mi mathvariant="double-struck">E</mi>
            </mrow>
            <mo stretchy="false">(</mo>
            <msub>
              <mi>R</mi>
              <mi>a</mi>
            </msub>
            <mo>&#x2212;</mo>
            <msub>
              <mi>R</mi>
              <mi>b</mi>
            </msub>
            <mo stretchy="false">)</mo>
          </mrow>
          <msqrt>
            <mtext>Var</mtext>
            <mo stretchy="false">(</mo>
            <msub>
              <mi>R</mi>
              <mi>a</mi>
            </msub>
            <mo>&#x2212;</mo>
            <msub>
              <mi>R</mi>
              <mi>b</mi>
            </msub>
            <mo stretchy="false">)</mo>
          </msqrt>
        </mfrac>
      </mtd>
    </mtr>
  </mtable>
</math>
Where \(R_a\) and \(R_b\) are the returns of two assets. The strategy is to go long on asset \(a\) and short on asset \(b\) when the strategy coefficient \(S\) is above a certain threshold, and to do the opposite when \(S\) is below a certain threshold.
The ratio compares the mean average of the excess returns of the asset or strategy with the standard deviation of those returns. Thus a lower volatility of returns will lead to a greater Sharpe ratio, assuming identical returns.

The "Sharpe Ratio" often quoted by those carrying out trading strategies is the annualised Sharpe, the calculation of which depends upon the trading period of which the returns are measured. Assuming there are 
 trading periods in a year, the annualised Sharpe is calculated as follows:
 <math xmlns="http://www.w3.org/1998/Math/MathML" display="block">
  <mtable displaystyle="true" columnalign="right" columnspacing="" rowspacing="3pt">
    <mtr>
      <mtd>
        <msub>
          <mi>S</mi>
          <mi>A</mi>
        </msub>
        <mo>=</mo>
        <msqrt>
          <mi>N</mi>
        </msqrt>
        <mfrac>
          <mrow>
            <mrow data-mjx-texclass="ORD">
              <mi mathvariant="double-struck">E</mi>
            </mrow>
            <mo stretchy="false">(</mo>
            <msub>
              <mi>R</mi>
              <mi>a</mi>
            </msub>
            <mo>&#x2212;</mo>
            <msub>
              <mi>R</mi>
              <mi>b</mi>
            </msub>
            <mo stretchy="false">)</mo>
          </mrow>
          <msqrt>
            <mtext>Var</mtext>
            <mo stretchy="false">(</mo>
            <msub>
              <mi>R</mi>
              <mi>a</mi>
            </msub>
            <mo>&#x2212;</mo>
            <msub>
              <mi>R</mi>
              <mi>b</mi>
            </msub>
            <mo stretchy="false">)</mo>
          </msqrt>
        </mfrac>
      </mtd>
    </mtr>
  </mtable>
</math>

##Limitations

The Sharpe ratio has several limitations that should be considered when using it to evaluate the performance of an investment or trading

##Benchmark for our strategy - IMOEX
so need to update SNP.csv database to IMOEX.csv database

##A bit of structure

DataHandler → MarketEvent → Strategy → SignalEvent → Portfolio → OrderEvent → ExecutionHandler