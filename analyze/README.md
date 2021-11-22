# Python scripts for analysis

This folder provides several python scripts to make analyzing and comparing the measured data (from the AD HiL system) and simulated data (from the OMNeT model) easier.

The core idea behind the scripts is to first convert them into a common datastructure.
This makes plotting and comparing them much easier.

## Reading in the Data
the two core scripts `read_measured.py` and `read_simulation.py` read in provided CSV files and map them into the following data structure:

`result:Dict(timestamp:String, Array:Float)`

for example:
```python
{
    'B0': [32.0, 32.0, 32.0, 32.0, 32.0],
    'B1': [0.0, 0.0, 0.0, 0.0, 0.0],
    'B2': [32.0, 32.0, 32.0, 32.0, 32.0],
    'B3': [47.0, 47.0, 47.0, 47.0, 47.0],
    'T0': [0.0, 0.01, 0.02, 0.03, 0.04],
    'T1': [0.0, 0.009003421, 0.019036095, 0.029040064, 0.039036946],
    'T2': [0.0, 0.008916953, 0.018985091, 0.029028868, 0.039029702],
    'T3': [0.0, 0.008860722, 0.018928369, 0.028972534, 0.038972418],
    'dT0': [0.01, 0.01, 0.01, 0.01, 0.01],
    'dT1': [0.009003421, 0.010032674, 0.010003969, 0.009996882, 0.010002851],
    'dT2': [0.008916953, 0.010068138, 0.010043777, 0.010000834, 0.009924362],
    'dT3': [0.008860722, 0.010067647, 0.010044165, 0.009999884, 0.009923654],
    ...
}
```

The two scripts export these datastructures as `results`.

There is also the option of using slightly cleaned up versions of the results.
These will have zero values removed for example where they don't make sense.
These are exported as `clean_results`.

## Using the Data
just import the data as follows:
```python
from read_simulation import results as sr
from read_measured import results as mr
```
and use them to analyze, plot or compare as you please.

### Existing Scripts
I will now provide a short description of the existing python scripts (besided the read scripts):
- `bar_cpu_usage_simulation.py`: plots the simulated cpu usage as a bar chart. One bar for each core. (limited to one pc)
- `gantt_scheduler.py`: generate a gantt chart of the simulated tasks (on any core) (limited to one pc)
- `mapping.py`: helper file for `read_simulated.py`
- `plot_cpu_usage_measured.py`: plots the measured CPU usage from the kscc.csv files
- `plot_cpu_usage_simulation.py`: plots the simulated CPU usage from OMNeT's exported output.json file (all cores, all PCs)
- `plot_multiple_cpu_usage_simulation_verification.py`: plots simulated cpu usage in different probing intervals, used for verification in my thesis
- `plot_timestamp_diff_all.py`: plots the diff timings (dT`x`) for simulated, measured and as a boxplot
- `plot_timestamp_subtraction_all.py`: plots the latency times (T`x` - T`x-1`) for simulated, measured and as a boxplot
- `generate_traces.py`: generates traces from measured CSV data.


# Useful stuff:
- `awk '!seen[$0]++' kscc26398.csv > kscc26398_nodup.csv`: removes duplicate entries from the kscc CPU usage files.
- `simulations/<sim>/export.sh` removes long lines and exports the statistic vectors

# Bugs
- `generate_traces.py` sometimes generates negative values
