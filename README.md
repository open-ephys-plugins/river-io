# River I/O

This is an Open Ephys GUI plugin that streams spikes and events using the [River library](https://pbotros.github.io/river/docs/intro.html). River is a C++ framework that allows simple, schema’d streaming of data, without any compromises on latency or throughput. It requires a connection to a [Redis](https://redis.io/) database in order to work.

This plugin was developed by Paul Botros in the Carmena Lab at UC Berkeley. See [this fork](https://github.com/carmenalab/plugin-GUI/) of the GUI for the original implementation.

## Installation (currently Windows only)

### Dependencies (automatically installed at build time)

- [River](https://pbotros.github.io/river/docs/intro.html) - shared lib for Windows is included with this plugin
- [Redis](https://redis.io/) - shared lib for Windows is included with this plugin

## Usage

The plugin must be able to communicate with a Redis database. On Windows, this must be run using Windows Subsystem for Linux. Installation instructions can be found here: https://redis.io/docs/getting-started/installation/install-redis-on-windows/

