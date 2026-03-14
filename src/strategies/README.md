## Strategy description

SharpRatio - сколько доходности стратегия приносит на единицу риска -> it is calculated afetr implementing the strategy

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

## Limitations

The Sharpe ratio has several limitations that should be considered when using it to evaluate the performance of an investment or trading

##Benchmark for our strategy - IMOEX
so need to update SNP.csv database to IMOEX.csv database

##A bit of structure

DataHandler → MarketEvent → Strategy → SignalEvent → Portfolio → OrderEvent → ExecutionHandler

## MACD formulas
MACD Indicator

The Moving Average Convergence Divergence (MACD) indicator is based on the difference between two Exponential Moving Averages (EMA) of the price.

Exponential Moving Average (EMA)

The exponential moving average is defined recursively as:

<math display="block">
  <mrow>
    <msub>
      <mi>EMA</mi>
      <mi>t</mi>
    </msub>
    <mo>=</mo>
    <mrow>
      <msub>
        <mi>P</mi>
        <mi>t</mi>
      </msub>
      <mo>&#x22C5;</mo>
      <mi>k</mi>
    </mrow>
    <mo>+</mo>
    <mrow>
      <msub>
        <mi>EMA</mi>
        <mrow>
          <mi>t</mi>
          <mo>&#x2212;</mo>
          <mn>1</mn>
        </mrow>
      </msub>
      <mo>&#x22C5;</mo>
      <mo stretchy="false">(</mo>
      <mn>1</mn>
      <mo>&#x2212;</mo>
      <mi>k</mi>
      <mo stretchy="false">)</mo>
    </mrow>
  </mrow>
</math>

where

$P_t$ — closing price at time $t$

$EMA_{t-1}$ — previous EMA value

$k$ — smoothing factor

The smoothing factor is defined as

<math display="block">
  <mrow>
    <mi>k</mi>
    <mo>=</mo>
    <mfrac>
      <mn>2</mn>
      <mrow>
        <mi>N</mi>
        <mo>+</mo>
        <mn>1</mn>
      </mrow>
    </mfrac>
  </mrow>
</math>
	​

where

$N$ — the period of the moving average.

## STOP_LOSS strategy

The stop-loss strategy is a risk management technique used in trading to limit potential losses on an investment. It involves setting a predetermined price level at which a position will be automatically closed if the market moves against the trader. This helps to protect the trader from significant losses and allows them to manage their risk effectively.
### Entry Condition (Position Opening)
<math display="block">
  <mrow>
    <msub>
      <mi>P</mi>
      <mi>t</mi>
    </msub>
    <mo>&gt;</mo>
    <mfrac>
      <mi>SL</mi>
      <mi>&#x03B1;</mi>
    </mfrac>
  </mrow>
</math>

### Initial Stop-Loss Level
<math display="block">
  <mrow>
    <msub>
      <mi>SL</mi>
      <mi>t</mi>
    </msub>
    <mo>=</mo>
    <mi>&#x03B1;</mi>
    <mo>&#x22C5;</mo>
    <msub>
      <mi>P</mi>
      <mi>t</mi>
    </msub>
  </mrow>
</math>

### Trailing Stop Update
<math display="block">
  <mrow>
    <msub>
      <mi>SL</mi>
      <mi>t</mi>
    </msub>
    <mo>=</mo>
    <mi>max</mi>
    <mo stretchy="false">(</mo>
    <msub>
      <mi>SL</mi>
      <mrow>
        <mi>t</mi>
        <mo>&#x2212;</mo>
        <mn>1</mn>
      </mrow>
    </msub>
    <mo>,</mo>
    <mi>&#x03B1;</mi>
    <mo>&#x22C5;</mo>
    <msub>
      <mi>P</mi>
      <mi>t</mi>
    </msub>
    <mo stretchy="false">)</mo>
  </mrow>
</math>

### Exit Condition (Position Closing)
<math display="block">
  <mrow>
    <msub>
      <mi>P</mi>
      <mi>t</mi>
    </msub>
    <mo>&#x2264;</mo>
    <msub>
      <mi>SL</mi>
      <mi>t</mi>
    </msub>
  </mrow>
</math>