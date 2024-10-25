+++
title = "Hosting a Python Server"
description = "A recipe for running a Python-based web server available to the public internet."
date = "2024-10-21"
+++

# Python App

- I've been using [FastAPI](https://fastapi.tiangolo.com/) and [Uvicorn](https://www.uvicorn.org/) for my apps.
	You can refer to their documentation for setting up a web server.
- I've also been taken to using [htmx](https://htmx.org/) and [Jinja](https://jinja.palletsprojects.com/en/3.1.x/) for simple websites, rather than using a frontend framework like React (or similar), for the sake of simplicity.
- I would also recommend setting up your project to work with [uv](https://github.com/astral-sh/uv), because it will make deploying your code much, much easier.
- Important for your deployed environment: you don't want `uvicorn` listening to a public port, instead you want to proxy traffic through `nginx`.
- I have the following code set up, so I can choose the right `uvicorn` configuration for the context.
	- In local dev (when `PROFILE` is not set) it listens on TCP at `127.0.0.1:8080`.
	- In prod it instead listens to a Unix domain socket at `/tmp/uvicorn.sock`.
		- Keep this in mind! When we get to Nginx configuration this is going to come back!

```python
import os

import uvicorn


def main() -> None:
    profile = os.environ.get("PROFILE", "dev")
    match profile:
        case "dev":
            # Used in local dev, so you can connect through your browser.
            uvicorn.run(
                "yarn_finder.wsgi:app",
                host="127.0.0.1",
                port=8080,
                reload=True,
            )

        case "prod":
            # Used in prod, for Nginx.
            uvicorn.run(
                "yarn_finder.wsgi:app",
                uds="/tmp/uvicorn.sock",
            )


if __name__ == "__main__":
    main()
```

You can find an up-to-date version [in the GitHub repo for this project](https://github.com/crockeo/yarn-finder/blob/main/yarn_finder/main.py).

# Getting a Server

- You can easily provisiong compute from [Amazon Lightsail](https://aws.amazon.com/free/compute/lightsail/).
  Other compute will work as well, so feel free to choose your own compute here!
- If using Lightsail you will only have `:80` exposed by default.
  You will need to expose `:443` as well for when you set up HTTPS:
	- Go to Amazon Lightsail.
	- Click on your instance.
	- Click on `Networking`.
	- Scroll to `IPv4 Firewall` and add click `+ Add rule`.
	- Choose `HTTPS` for the `Application`, and then click `Create`.
- Set up SSH:
	- Will have more information on this, Soon™, but this is well documented on the internet.
	- You can go and find how to connect to your Lightsail instance over SSH, or you can use the web browser.

# Nginx

- I was using Ubuntu for this project, so going to talk about configuring this for Ubuntu. Your mileage may vary when using other operating systems.
- Install Nginx: `sudo apt-get update && sudo apt-get install nginx`. This will start `nginx` by default so you can go to `http://<your IP>` and see the default `nginx` screen.
- And then you can set a sane default configuration for your app by replacing the content in `/etc/nginx/sites-enabled/default`:

```nginx.conf
server {
	client_max_body_size 4G;

	server_name <your URL>;

	location / {
		proxy_set_header Host $http_host;
		proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
		proxy_set_header X-Forwarded-Proto $scheme;
		proxy_set_header Upgrade $http_upgrade;
		proxy_set_header Connection $connection_upgrade;
		proxy_redirect off;
		proxy_buffering off;
		proxy_pass http://uvicorn;
	}
}

map $http_upgrade $connection_upgrade {
	default upgrade;
	'' close;
}

# Note: this is where the uvicorn.sock thing comes back!
upstream uvicorn {
	server unix:/tmp/uvicorn.sock;
}
```

- And then you need to restart nginx: `sudo systemctl reload nginx`.
- You can go to the IP address of the box you've provisioned
  and you should a "502 Bad Gateway" page.

# DNS Configuration

- The specific actions you have to take are going to vary a lot by your specific DNS provider. I'll talk about Squarespace, but first talk about the task in generic terms.
- I'm assuming you're hosting on a subdomain, but most of these steps will be similar no matter what.
- DNS configuration:
	- You're going to want to add an `A` record where your `Host` is your subdomain name (e.g. `yarnfinder`) pointing to your IPv4 IP address (in the form `A.B.C.D`).
	- And then do the same for an `AAAA` record pointing to your IPv6 address (the long one with tons of `:`s).
- On Squarespace, you do this under the `Custom records` section. Click `Add Record` and then you can do the rest of what I've said here.
	- Note that if you're hosting this on a top-level domain managed by Squarespace, you should set the `Host` to `@`.

# HTTPS Configuration

- TL;DR: [certbot](https://certbot.eff.org/instructions)
- I followed the [Nginx + Linux (snap)](https://certbot.eff.org/instructions?ws=nginx&os=snap) instructions piece by piece and it all worked for me. You should be able to do the same--it's super easy to set up.
- If you get an error saying that it can't install because of a server name,
  you've probably forgotten to replace `<your URL>`.
	Try replacing it with your domain name :)
- And finally: `sudo systemctl reload nginx`.

# Running Your Code

- At this point if you go to your website you're going to get a very nice HTTPS-backed HTTP 503 error, because your server isn't running.
- There are a couple of things we have to do to get your code running:
	- Get a copy of your code to the machine.
	- Set up [uv](https://github.com/astral-sh/uv) on the remote machine.
	- Get your server running.

## Getting code to the cloud

- My project [happens to be open source](https://github.com/crockeo/yarn-finder) so this is easy: I just `git clone <url>` to get it onto the machine.
- If you want your project to be closed source, you could set it up in a couple of ways:
	- [Make an SSH key on your Lightsail instance](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent), and use that to clone from a private repository.
	- Or if you set up SSH between your local machine and the Lightsail instance, you could use [scp](https://linux.die.net/man/1/scp).
	- I'm going to leave definitive guidance up to the reader.

## Setting up `uv`

- [This is so simple](https://github.com/astral-sh/uv?tab=readme-ov-file#installation).
- TL;DR: `curl -LsSf https://astral.sh/uv/install.sh | sh`
	- You should probably download it and inspect it first as well :)

## Get your server running

- I'm assuming you're in your working directory, and your `main.py` file (the thing pasted in this blog post) is in `<your_project>/main.py`.
- There are multiple ways to run your server, some are better and some are easier. I'm going to go for the easiest option: `screen`.
	- If you want to know how to do this better, you can read [other posts on the internet](https://medium.com/@benmorel/creating-a-linux-service-with-systemd-611b5c8b91d6)!
- Just run it with 2 commands:
	- `PROFILE=prod screen uv run python -m <your_project>/main.py` (to run your server under `screen`).
	- `Ctrl+A` -> `D` (to detach you from the `screen` session).
	
