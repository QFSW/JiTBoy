set timing=%1
if not defined timing set timing=accurate

Release\JiTBoy.exe --timing=%timing% && python3 analysis\main.py