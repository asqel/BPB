import os

root = os.path.abspath("./disk")
print(root)

def dir_to_dict(path):
    result = {}

    for entry in os.scandir(path):
        if entry.is_dir():
            result[entry.name] = dir_to_dict(entry.path)
        elif entry.is_symlink():
            link = os.readlink(entry.path)
            if link.startswith(root):
                link = link.removeprefix(root)
                if not link.startswith("/"):
                    link = "/" + link
            result[entry.name] = (1, link)
        elif entry.is_file():
            result[entry.name] = entry.path

    return result



def to_bin(tree: dict) -> bytes:
    result = b''

    for i in tree.keys():
        if type(tree[i]) == str:
            result += (ord('F')).to_bytes(1, "little", signed = False)
            result += i.encode() + b'\0';
            f = open(tree[i], "rb")
            content = f.read()
            result += len(content).to_bytes(4, "little", signed = False)
            result += content
        elif type(tree[i]) == tuple:
            result += (ord('L')).to_bytes(1, "little", signed = False)
            result += i.encode() + b'\0';
            result += tree[i][1].encode() + b'\0'
        else:
            result += (ord('D')).to_bytes(1, "little", signed = False)
            result += i.encode() + b'\0';
            content = to_bin(tree[i])
            result += len(content).to_bytes(4, "little", signed = False)
            result += content

    return result


with open("disk.bin", "wb") as f:
    f.write(to_bin(dir_to_dict("disk/")))