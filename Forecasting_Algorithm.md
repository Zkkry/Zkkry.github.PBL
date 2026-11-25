# Documentation on Forecasting Algorithm Selection

## 1.0 Introduction

The IoT Sensor Data Forecasting System is developed to monitor and predict temperature and humidity using an ESP32 microcontroller and a DHT11 sensor. The sensor collects environmental data every 10 minutes and transmits the readings to Google Sheets through a wireless internet connection.

Google Apps Script processes the historical sensor data and generates temperature and humidity predictions for the next 24 hours. The actual readings, forecast values, uncertainty bounds and forecasting accuracy are visualized through a Looker Studio dashboard.

The main objective of forecasting is to estimate future environmental conditions based on previously collected sensor readings.

## 2.0 Research on Forecasting Techniques

### 2.1 Simple Moving Average (SMA)

Simple Moving Average is a forecasting technique that calculates the average of a selected number of historical observations. Each observation is given equal importance when calculating the forecast value.

The SMA formula is:

**SMA = (X₁ + X₂ + ... + Xₙ) / n**

Where:

- X represents the historical sensor readings.
- n represents the number of observations.
- SMA represents the predicted value.

**Advantages:**

- Simple to understand and implement.
- Requires minimal computational resources.
- Reduces the effect of short-term fluctuations.
- Suitable for relatively stable temperature and humidity readings.
- Easy to integrate with Google Apps Script.

**Limitations:**

- Responds slowly to sudden environmental changes.
- Gives equal importance to all observations within the selected window.
- Does not directly identify trends or seasonal patterns.

### 2.2 Exponential Moving Average (EMA)

Exponential Moving Average is a forecasting technique that assigns greater importance to recent observations while gradually reducing the importance of older observations.

The EMA formula is:

**EMAₜ = αXₜ + (1 − α)EMAₜ₋₁**

Where:

- EMAₜ represents the current exponential moving average.
- Xₜ represents the current observation.
- EMAₜ₋₁ represents the previous exponential moving average.
- α represents the smoothing factor.

**Advantages:**

- Responds more quickly to recent environmental changes.
- Reduces the influence of older observations.
- Suitable for continuously updated sensor data.

**Limitations:**

- Requires an appropriate smoothing factor.
- More sensitive to sudden fluctuations when the smoothing factor is high.
- Slightly more complex to understand and configure than SMA.

## 3.0 Comparison of Forecasting Algorithms

| Criteria | Simple Moving Average (SMA) | Exponential Moving Average (EMA) |
|---|---|---|
| Calculation complexity | Low | Moderate |
| Data weighting | Equal weighting | Higher weighting for recent data |
| Response to changes | Slower | Faster |
| Computational requirements | Low | Low |
| Ease of implementation | Very easy | Easy |
| Parameter configuration | Moving average window | Smoothing factor |
| Google Apps Script compatibility | Suitable | Suitable |
| Suitability for stable sensor readings | High | High |

## 4.0 Selected Forecasting Algorithm

The Simple Moving Average (SMA) algorithm was selected for the implementation of this IoT Sensor Data Forecasting System.

SMA was selected because of its simplicity, low computational requirements and suitability for processing temperature and humidity readings. It can be implemented using Google Apps Script without requiring additional machine learning libraries or complex mathematical models.

The algorithm calculates forecast values using historical sensor observations. These forecast values are used to generate predictions for the next 24 hours.

Although EMA can respond more quickly to recent changes, SMA provides a straightforward and understandable forecasting approach that is appropriate for the scope of this project.

## 5.0 Justification of Algorithm Selection

### 5.1 Simplicity

The SMA algorithm uses a basic arithmetic average, making it easier to understand, implement, troubleshoot and explain during the project demonstration.

### 5.2 Computational Efficiency

SMA requires relatively few mathematical operations. Therefore, it can process historical sensor readings efficiently using Google Apps Script.

### 5.3 Suitability for Environmental Data

Temperature and humidity readings may experience short-term fluctuations. SMA helps smooth these fluctuations and provides a baseline prediction based on historical environmental conditions.

### 5.4 System Integration

SMA can be integrated directly with Google Sheets and Google Apps Script. The generated forecast values can be stored in a separate worksheet and displayed automatically through Looker Studio.

### 5.5 Ease of Maintenance

The algorithm is easy to maintain and modify. Its moving average window can be adjusted according to the amount of available historical sensor data and the desired responsiveness.

## 6.0 Forecasting Implementation

The forecasting process consists of the following steps:

1. The DHT11 sensor measures temperature and humidity.
2. The ESP32 transmits sensor readings to Google Sheets every 10 minutes.
3. Google Apps Script retrieves and processes valid historical sensor readings.
4. Historical readings are organized into hourly averages.
5. The SMA algorithm calculates temperature and humidity forecast values.
6. Predictions are generated for the next 24 hours.
7. Upper and lower uncertainty bounds are calculated.
8. Forecast results are stored in the Forecasts worksheet.
9. Looker Studio displays the actual readings, forecast values and forecasting accuracy.

The implemented SMA approach produces a constant forecast across the 24-hour prediction horizon until the forecast is recalculated using updated historical data. This behaviour is expected because the model uses a historical average rather than a trend-based or seasonal forecasting method.

## 7.0 Forecasting Accuracy Evaluation

The performance of the selected forecasting algorithm is evaluated using Mean Absolute Error (MAE) and Mean Absolute Percentage Error (MAPE).

### 7.1 Mean Absolute Error (MAE)

MAE measures the average absolute difference between the actual sensor readings and predicted values.

**MAE = (Σ |Actual − Forecast|) / n**

A lower MAE indicates that the forecast values are closer to the actual sensor readings.

### 7.2 Mean Absolute Percentage Error (MAPE)

MAPE measures the average absolute forecasting error expressed as a percentage of the actual values.

**MAPE = (100 / n) × Σ |(Actual − Forecast) / Actual|**

Observations with zero actual values must be excluded from the percentage error calculation.

A lower MAPE indicates better forecasting performance. Both MAE and MAPE are displayed on the Looker Studio dashboard and updated as additional forecasts are evaluated against actual sensor readings.

A larger number of evaluated forecasts is necessary to obtain a more reliable assessment of forecasting performance.

## 8.0 Limitations of the Selected Algorithm

Although SMA is suitable for this project, it has several limitations. The algorithm cannot directly identify seasonal patterns, sudden environmental changes or long-term trends.

The accuracy of the forecast also depends on the quality, availability and consistency of historical sensor data. Internet connection problems, missing sensor readings and sensor measurement errors may affect forecasting performance.

Furthermore, the constant 24-hour forecast may not represent actual hourly environmental variations. The upper and lower bounds provide an approximate indication of uncertainty rather than a guarantee that future readings will remain within the predicted range.

More advanced forecasting techniques may be considered in future system improvements.

## 9.0 Conclusion

In conclusion, Simple Moving Average was selected as the forecasting algorithm for the IoT Sensor Data Forecasting System because of its simplicity, computational efficiency, ease of implementation and compatibility with Google Apps Script.

The algorithm enables the system to generate 24-hour temperature and humidity predictions using historical sensor readings. The integration of ESP32, DHT11, Google Sheets, Google Apps Script and Looker Studio provides a practical platform for environmental monitoring, forecasting and performance evaluation.

Although SMA has limitations in predicting sudden changes and seasonal patterns, it provides a suitable baseline forecasting method for the scope and objectives of this project.
