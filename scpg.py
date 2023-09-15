# wrote that for my friend
import typing

from paramiko import SSHClient, AutoAddPolicy
from scp import SCPClient

import os
from threading import Thread

import tkinter
from tkinter import ttk
from tkinter.filedialog import askopenfilename
import sv_ttk

base_dir: str = "/"

localf_path:  tkinter.StringVar
targetf_path: tkinter.StringVar

progressv: tkinter.IntVar
progress: ttk.Progressbar

uname:  ttk.Entry
passwd: ttk.Entry
host:   ttk.Entry

def progress_callback(_: str, sz: int, sent: int) -> None:
  global progress, progressv
  progress.config(maximum=sz)
  progressv.set(sent)
  print(sent)

def get_localf() -> None:
  global localf_path, targetf_path, base_dir
  localf_path.set(askopenfilename())
  targetf_path.set(base_dir + os.path.basename(localf_path.get()))

def cpy() -> None:
  global localf_path, targetf_path
  global uname, passwd, host

  ssh = SSHClient()
  ssh.set_missing_host_key_policy(AutoAddPolicy())
  ssh.load_system_host_keys()
  ssh.connect(hostname=host.get(), username=uname.get(), password=passwd.get())

  scp = SCPClient(ssh.get_transport(), progress=progress_callback)

  scp.put(files=localf_path.get(), remote_path=targetf_path.get())

  scp.close()
  ssh.close()

def main() -> None:
  global localf_path, targetf_path
  global uname, passwd, host
  global progressv, progress
  global base_dir
  root = tkinter.Tk()

  root.title("scpg")
  root.resizable(width=False, height=False)

  localf_path = tkinter.StringVar()
  targetf_path = tkinter.StringVar()

  progressv = tkinter.IntVar()

  localf_path.set("")
  targetf_path.set(base_dir)

  label_uname: ttk.Label = ttk.Label(master=root, text="username")
  uname = ttk.Entry(master=root)
  uname.insert("end", "")
  label_uname.pack(fill="both", expand=False, padx=5)
  uname.pack(fill="both", expand=False, padx=5, pady=5)

  label_passwd: ttk.Label = ttk.Label(master=root, text="password")
  passwd = ttk.Entry(master=root, show='*')
  passwd.insert("end", "")
  label_passwd.pack(fill="both", expand=False, padx=5)
  passwd.pack(fill="both", expand=False, padx=5, pady=5)

  label_host: ttk.Label = ttk.Label(master=root, text="host")
  host = ttk.Entry(master=root)
  host.insert("end", "")
  label_host.pack(fill="both", expand=False, padx=5)
  host.pack(fill="both", expand=False, padx=5, pady=5)

  ttk.Separator(master=root).pack(fill="both", pady=10)

  localf: ttk.Button = ttk.Button(master=root, text="Select local file",
                           command=get_localf)
  localf.pack(fill="both", expand=False, padx=5, pady=10)

  label_targetf: ttk.Label = ttk.Label(master=root, text="target file path")
  targetf = ttk.Entry(master=root, textvariable=targetf_path)
  label_targetf.pack(fill="both", expand=False, padx=5)
  targetf.pack(fill="both", expand=False, padx=5, pady=5)
  copy_ok: ttk.Button = ttk.Button(master=root, text="Copy",
                                   command=lambda: Thread(target=cpy).start())
  copy_ok.pack(fill="both", expand=False, padx=5, pady=5)

  progress = ttk.Progressbar(mode='determinate', master=root,
                             variable=progressv)
  progress.pack(fill="both", pady=5)

  sv_ttk.set_theme("dark")
  root.mainloop()

if __name__ == "__main__":
  main()
