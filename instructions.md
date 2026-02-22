# QuantPy instructions

## Installation

The `QuantPy` library is meant to be used on a Linux distribution (or WSL on Windows) and the instructions assume such. Much of the instructions could be applied to other platforms with proper domain knowledge.

The `QuantPy` library is designed to be used with Python 3.12 and C++17. The setup scripts assume that GNU compiler is available for C++. 

Before the setup some steps need to be taken. The commands in the instructions should be ran in the root quantform directory where these instructions are located as well. The steps are

0. Install Python 3.12 and C++17 compiler. With Ubuntu or WSL this can be done by running the following commands in the terminal

    ```
    sudo add-apt-repository ppa:deadsnakes/ppa
    ```
    ```
    sudo apt update
    ```
    ```
    sudo apt install python3.12-full -y
    ```
    ```
    sudo apt-get update
    ```
    ```
    sudo apt-get install g++
    ```

1. Create a virtual environment by running the following commands in the terminal

    ```
    python3.12 -m venv .venv
    ```
    ```
    source .venv/bin/activate
    ```

## Setup

These steps need to be taken each time modifications are made to the source code. Once again these instructions are assumed to be ran from the root quantpy directory. 

0. Activate the virtual environment by running the command 

    ```
    source .venv/bin/activate
    ```

1. Install the `QuantPy` library using pip. By using the `--editable` flag changes in the Python code are will be reflected in the installed package without a need for a reinstall. However, for the changes in the C++ code to take effect the install needs to be redone. 

    ```
    python3.12 -m pip install --editable .
    ```
