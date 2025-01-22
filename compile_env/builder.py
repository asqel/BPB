import os

path = os.path.dirname(os.path.abspath(__file__))
out = os.path.join(path, "out")

dirs = [ \
	os.path.join(path, d) for d in os.listdir(path) \
	if os.path.isdir(os.path.join(path, d)) and os.path.join(path, d).endswith(".dir") \
]

os.makedirs(out, exist_ok=True)

# check if _start_file.o exists
if not os.path.exists(os.path.join(path, "_start_file.o")):
	os.system(f"sh {os.path.join(path, 'gcc')} -c {os.path.join(path, '_start_file.c')} -o {os.path.join(path, '_start_file.o')}")

def get_files(dir : str) -> list[str]:
	files = []
	for f in os.listdir(dir):
		real_path = os.path.join(dir, f)
		if os.path.isdir(real_path):
			files.extend(get_files(real_path))
		elif real_path.endswith(".c"):
			files.append(real_path)
	return files


def get_flags(dir : str) -> str:
	if not os.path.exists(os.path.join(dir, "flags")):
		return ""
	with open(os.path.join(dir, "flags"), "r") as f:
		return f.read()

def compile_dir(dir):
	out_name = os.path.join(out, os.path.basename(dir).removesuffix(".dir"))

	files = get_files(dir)
	bins = files.copy()
	for i in range(len(bins)):
		bins[i] = files[i].removesuffix(".c") + ".o"
	flags = get_flags(dir)
	for i in range(len(files)):
		if not os.path.exists(bins[i]) or os.path.getmtime(bins[i]) < os.path.getmtime(files[i]):
			cmd = f"sh {os.path.join(path, 'gcc')} -c {files[i]} -o {bins[i]} {flags}"
			os.system(cmd)

	del files
	all_files = ""
	for i in range(len(bins)):
		all_files += f" {bins[i]}"
	all_files += " _start_file.o"

	cmd = f"sh {os.path.join(path, 'gcc')} {all_files} -o {out_name} --shared"
	os.system(cmd)


for i in range(len(dirs)):
	compile_dir(dirs[i])