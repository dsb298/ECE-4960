from ctypes.wintypes import HACCEL
import numpy as np
import matplotlib.pyplot as plt
from scipy import pi
from scipy.fftpack import fft
from scipy import signal
import pandas as pd

df = pd.read_csv('accel_data.csv')
df_pitch  = df['pitch']
df_roll   = df['roll']
pitch_arr = df_pitch.to_numpy()
roll_arr  = df_roll.to_numpy()
pitch_arr = np.resize(pitch_arr, (1480,))
roll_arr  = np.resize(roll_arr, (1480,))

sample_rate = 296
N = (5 - 0) * sample_rate

time = np.linspace(0, pitch_arr.size, N)

# freq1 = 60
# magnitude1 = 25
# freq2 = 270
# magnitude2 = 2

# waveform1 = magnitude1 * np.sin (2 * pi * freq1 * time)
# waveform2 = magnitude2 * np.sin (2 * pi * freq2 * time)
# noise = np.random.normal (0, 3, N)

time_data = pitch_arr + roll_arr

# plt.plot (time [0:], time_data [0:])
# plt.title ('Time Domain Signal')
# plt.xlabel ('Time')
# plt.ylabel ('Amplitude')
# plt.show ()

###########################################

frequency = np.linspace (0.0, sample_rate/2, int (N/2))

freq_data = fft(time_data)
freq_data[0] = 0
y = 2/N * np.abs (freq_data [0:np.int (N/2)])

# plt.plot(frequency, y)
# plt.title('Frequency domain Signal')
# plt.xlabel('Frequency in Hz')
# plt.ylabel('Amplitude')
# plt.show()

w,h = signal.freqz(freq_data)

plt.plot(w, 20 * np.log10(abs(h)))
plt.title('Frequency response')
plt.xlabel('Frequency')
plt.ylabel('Amplitude')
plt.show()