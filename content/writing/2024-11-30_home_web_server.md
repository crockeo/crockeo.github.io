+++
title = "Home Webserver"
description = "Buying a used Mac Mini, setting up a home server."
date = "2024-11-30"
+++

I bought a used Mac Mini on Craigslist recently for about $200. I've been wanting to set up a home server for quite a while, and it seemed like a good entrypoint into the space! It has a couple of nice things for such a cheap price:

- A 3.00GHz Intel i7-4578U.
- A nice 16GB of RAM.
- About 100GB of HDD (yes, a real spinning disk!)

It's not a powerhouse by any means, but it's been powerful enough to set up a couple of services, like:

- A [Minecraft](https://www.minecraft.net/en-us) server.
- A [Factorio](https://www.factorio.com/) server.
- [Prometheus](https://github.com/prometheus/prometheus) and [Grafana](https://github.com/grafana/grafana), along with a [healthchecker for Minecraft](https://github.com/crockeo/minecraft-healthcheck), so I can be oncall for my hobby project (🙃).

I'm planning on using it to host a couple of websites as well, at least while I don't care too much about uptime.

# Installation & Networking
I installed [Ubuntu Server](https://ubuntu.com/download/server), which was surprisingly easy. Folks on the internet warned about the networking issues after install. The Broadcom chip that this Mac Mini uses doesn't have a networking driver installed by default. I just ran an Ethernet to the Mac when I set it up, installed the proprietary Broadcom driver, and then we were off to the races!

## Hibernate
By default Ubuntu Server was configured to sleep/hibernate when there wasn't much activity. That was surprising to me, because I meant to host the server 24/7! You can turn this off with:

```
sudo systemctl mask \
	hibernate.target \
	hybrid-sleep.target \
	sleep.target \
	suspend.target
```

And then the server will stay online 24/7!
## IPv6 Issues
Except that the Broadcom driver would SIGSEGV sometimes when using IPv6!

I ended up uninstalling the Broadcom driver, putting the server next to the router, and just running an Ethernet cable permanently. I wish I had a stack trace on hand, but it's long ago enough that it's not available in `/var/log/syslog` 😔.

## Mystery Networking Issues
And even still, sometimes the networking stack crashes! I've noticed this happens while using the Minecraft Bedrock server, but I haven't debugged further than that. You can fix it by restarting `NetworkManager`... but I can't `ssh` in while this is happening.

I've done a cursed thing by setting up a `systemd` service named `network-repair` which is just a cursed little script:

```python
#!/usr/bin/env python3.12
import logging
import subprocess
import time

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)


def main() -> None:
    addr = "8.8.8.8"
    while True:
        logger.info(f"Polling {addr}...")

        is_online = False
        try:
            subprocess.check_call(("ping", "-c", "1", "-w", "5", addr), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            is_online = True
        except Exception:
            logger.error(f"Failed to connect to {addr}", exc_info=True)
            is_online = False

        if not is_online:
            logger.warning("Lost connectivity. Attempting to restart NetworkManager.")
            try:
                subprocess.run(("systemctl", "restart", "NetworkManager"))
            except Exception:
                logger.error("Unable to restart NetworkManager", exc_info=True)

        time.sleep(30)


if __name__ == "__main__":
    main()
```

TL;DR: check if we have internet and, if we don't, try restarting our `NetworkManager`.

Eventually™ I'll figure out what's actually going wrong and fix it, but for now, this is working well enough! 30s of downtime every couple of days isn't so bad. That's still >99% uptime!

# `systemd` Services
I don't know how to write good `systemd` services, but I do know how to write `systemd` services. I've been using a basic template:

- Make a user with `sudo useradd --no-create-home --user-group <username>`.
- Make >=1 directory under `/opt` which will contain the content for the service.
	- E.g. I've added `/opt/minecraft`, to contain the Minecraft Bedrock server.
	- And then you have to `sudo chown -R <username>:<username> /opt/<directory>` to make the user own the directory.
- Create a `systemd` service file, and put it under `/etc/systemd/system/`:

```ini
[Unit]
Description=<name>
; If this needs the network:
After=network-online.target

[Service]
; Give each service its own user + group,
; which only has access to its content:
User=<username>
Group=<username>
Restart=on-failure
ExecStart=<executable + args>

; Can set `WorkingDirectory` if you need it:
WorkingDirectory=<path>

; And then you can also set environment variables:
Environment="<key>=<value>"

[Install]
WantedBy=multi-user.target
```

- Then turn it on:
	- To make it start on bootup: `systemctl enable <name>.service` (and its opposite: `systemctl disable`).
	- To make it start immediately: `systemctl start <name>.service` (and again, its opposite: `systemctl stop`). 
- And you can monitor it with `sudo journalctl -u <name>.service`.

# Monitoring
Like I said, I configured Grafana and Prometheus to set up monitoring and alerting. With the rest of this blog post you should have the skills to set them up at this point:

- [Grafana's install instructions](https://grafana.com/docs/grafana/latest/setup-grafana/installation/debian/) are pretty straightforward.
- Prometheus is a bit more complicated:
	- [You have to download compiled binaries from their site](https://prometheus.io/download/).
	- And then write your own `prometheus.service` to run it. I used [this blog post](https://janakiev.com/blog/prometheus-setup-systemd/) when setting it up initially.
- You can configure [Grafana to use Prometheus](https://grafana.com/docs/grafana/latest/datasources/prometheus/).
- And then use Grafana to [make some dashboards](https://grafana.com/docs/grafana/latest/dashboards/), and set up [email notification](https://grafana.com/docs/grafana/latest/alerting/configure-notifications/manage-contact-points/integrations/configure-email/).

And then BOOM! You can be oncall for your own hobby project. 🎉

# Admonition
This was all thrown together haphazardly, mostly from memory. If something here isn't working for you feel free to complain to me at [me@crockeo.net](mailto:me@crockeo.net). I'm happy to dig into it and issue a correction.