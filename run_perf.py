import subprocess

BENCH_BIN = '-- ./build/Release/example/workload/pool/example_workload_pool'

# sudo perf script | ../FlameGraph/stackcollapse-perf.pl | ../FlameGraph/flamegraph.pl  > workload.svg


def run_perf():
    try:
        command = ['perf', 'record', '-F', '99', '-g', BENCH_BIN]

        result = subprocess.run(command, capture_output=True, text=True)

        if result.stdout:
            print("Output:", result.stdout)

        if result.stderr:
            print("Error:", result.stderr)

        print("Return code:", result.returncode)

    except Exception as e:
        print("An error occurred:", str(e))


run_perf()