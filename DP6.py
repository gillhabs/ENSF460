"""
    Project: ENSF460_DP6
    File:   DP6.py
    Author: Halanna Le, Christy Guirguis, Gillian Habermehl
"""

import csv
import math
import time

import numpy as np
import pandas as pd
import plotly.express as px
import plotly.io as pio
import serial

pio.renderers.default = "browser" # plots plotly graphs on a browser window

## OPEN SERIAL PORT
ser = serial.Serial(port = "/dev/cu.usbserial-FT3V5KWM", baudrate = 9600, bytesize = 8, timeout = 2, stopbits = serial.STOPBITS_ONE) # opens serial port

## INITIALIZATION
rxNumsStr = ""
rxNumsList = []
rxTimesList = []


startTime = time.time() # records start time

while(time.time() - startTime < 10): # reads for a total of 10 seconds
    line = ser.readline() # reads single bytes (char) until \n is reached
    if ((line != b'\n') and (line != b' \n')):
        rxNumsStr = rxNumsStr + line.decode("Ascii")
        timeMeas = time.time() - startTime
        rxTimesList.append(timeMeas)

## CLOSE SERIAL PORT
ser.close()

## DATA CLEAN UP
rxNumsStr = rxNumsStr.strip() # removes any spaces or \n or \r from beginning and end of string
rxNumsList = rxNumsStr.split(" \n ")

## SPLIT DATA INTO BUFFER AND VOLTAGE VALUES
rxBuffer = [int(elem) for elem in rxNumsList]
rxVoltage = [(elem * 3.25 / 1023) for elem in rxBuffer]

## CONVERT Rx DATA INTO DATA FRAME
dF = pd.DataFrame()
dF["Time"] = rxTimesList
dF["Buffer"] = rxBuffer
dF["Voltage"] = rxVoltage

## COPY RX DATA IN CSV FILE
dF.to_csv("RxData.csv", index = True)

## PLOT DATA
fig = px.line(dF, x = "Time", y = "Buffer", title = "ADC Buffer vs time")
fig.show()

fig2 = px.line(dF, x = "Time", y = "Voltage", title = "ADC Voltage vs time")
fig2.show()