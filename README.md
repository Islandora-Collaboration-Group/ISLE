# ISLE: Islandora Enterprise

## What is ISLE?
[ISLE](https://github.com/Islandora-Collaboration-Group/ISLE) is a community project—initially funded by [17 academic institutions](https://docs.google.com/document/d/1ycx5ATbeWpUWvpZ6bwXws490CMgi0dyB9SBfPYUDEjk/edit?usp=sharing) and Born-Digital—that addresses two of the most significant pain-points in Islandora: installation and maintenance.

ISLE uses replaceable Docker images to streamline and largely automate the process of installing and maintaining the entire Islandora 7.x stack, while at the same time enabling institutions to create customizations that persist separately from core code. The result is the ability to easily, quickly and regularly update an institution’s entire Islandora stack. ISLE requires significantly less staff time and also reduces dependency on expert technical staff and outside vendors.

Perhaps most importantly, the ISLE [project maintainers](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group#project-maintainers) keep your Islandora stack smoothly running with minimal effort by regularly releasing updated (and tested) Docker images that contain up-to-date Islandora releases, software patches, security updates and feature improvements.

ISLE is quite flexible and may be run on an institution’s servers or in the cloud, or as a hybrid; similarly, it may be maintained by an institution’s staff, by a vendor, or as a shared project.

Looking ahead: ISLE is collaborating with the community’s [LASIR](https://github.com/Islandora-Collaboration-Group/LASIR) project to develop and include a robust suite of Institutional Repository (IR) features within Islandora. ISLE’s common, updatable platform will be advantageous to the community as we prepare to move toward the next major version of Islandora CLAW.

## ISLE Documents
* [ISLE Documentation](https://islandora-collaboration-group.github.io/ISLE-Documentation/) (user-friendly documentation)
* [ISLE Executive Summary](https://docs.google.com/document/d/17tAFxR6_b7sxXkE1teNDQZv0UZ0LLSkX8K05-U6A6nw/edit?usp=sharing) (project overview)

## How to Join
* [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody!  [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT
* [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Subscribe for updates, meeting announcements, and technical support

## Funding
- **ISLE Phase I:** $90,000 crowdfunded ($66,000 from 17 academic institutions; $24,000 in-kind labor from Born-Digital); exceeded original goal of $84,000.
- **ISLE Phase II:** Planning funding
- **ISLE Phase III:** TBD

## Timeline
- 08/2016 - [Completed] ISLE Phase I conceived with the ICG and Common Media/Born-Digital
- 09/2016 - [Completed] ISLE presented at ICG Hack/Doc @ Wesleyan University
- 05/2017 - [Completed] ISLE presented at Islandoracon 2017 annual conference (Hamilton, Ontario)
- 09/2017 - [Completed] $90,000 crowdfunded from 17 academic institutions to fund ISLE Phase I
- 09/2017 - [Completed] ISLE MVP: Opened 3-week Public Comment Period
- 12/2017 - [Completed] Public prototype demonstration: [view 5-minute video](https://vimeo.com/245777329)
- 12/2017 - [Completed] Alpha Test #1: Grinnell College (lead: Mark McFate)
- 04/2018 - [Completed] Alpha Test #2: Williams College (lead: David Keiser-Clark)
- 03/2018 - [Completed] Alpha Test #3: Rensselaer Polytechnic Institute (lead: Andrea Byrne)
- 03/2018 - [In Process] Community Alpha Test #1: University of Pittsburgh (lead: Clinton Graham)
- 03/2018 - [Completed] Community Alpha Test #2: Hamilton College (lead: Steve Young)
- 03/2018 - [Completed] Community Alpha Test #3: Barnard College (lead: Ben Rosner)
- 05/2018 - [In Process] ISLE Steering Committee approved ISLE (Islandora 7x) hand-off from Born-Digital
- 05/2018 - [Completed] ISLE v1.0 release available as open-source for public use
- 08/2018 - [In Process] ISLE v1.1 release available as open-source for public use
- 10/2018 - [Future] ISLE Phase II begins

## Why Use ISLE?
ISLE cuts in half (compared to pre-ISLE upkeep costs) the time required to install and maintain an Islandora stack. Clear documentation makes it a snap to test installation on a laptop, install it directly on a local host server or cloud service, or update a pre-existing ISLE site. ISLE standardizes best-practice security features and protocols, removing much of the opportunity for human error from configuration and upkeep of Production Islandora systems. Multiple stacks (production, staging, and development) may be installed on a single virtual server, and each may be distinctly configured. Initial use cases are showing ISLE to be dramatically faster (more on that soon).

Islandora is a powerful digital repository comprised of over 80 different, best-of-breed open-source software libraries. Islandora’s strength is that it integrates these packages into one easy to manage system. However, this ecosystem has traditionally been complex enough as to require staff with technical expertise to install and maintain.

ISLE lowers the barrier to entry for new institutions or organizations because it reduces the complexity and expense to the point where non-technical staff can maintain an Islandora stack in just a few hours per month. This in turn allows institutions to reallocate funds towards development or ingestion instead of maintenance. An ISLE maintained Islandora stack is secure and can be run as a production server straight out of the box.

A note about migration: ISLE contains complete documentation for migrating existing Islandora installations to the ISLE framework. Institutions currently using CONTENTdm, Digital Commons, or other platforms are able to migrate to ISLE by using the well-documented [Move to Islandora Kit](https://github.com/MarcusBarnes/mik) (MIK). It is assumed that some institutions will have custom theming, customizations and metadata formats that, while they are preserved during ISLE updates, will need to be manually refactored during the migration to CLAW.

## Project Sustainability
The ISLE Steering Committee (ISC) oversees the ISLE project and the ISLE Project Maintainers maintain the project and create scheduled new releases. The ISC will submit the resultant development via Islandora’s Licensed Software Acceptance Procedure ([LSAP](https://islandora.ca/developers/lsap)). The development will then become a core part of the Islandora 7x Release (Fall 2019) and will be supported and maintained by the Islandora community as part of the free and publicly shared open-source platform. Through this LSAP method, Islandora has already adopted a large number of externally contributed modules that are sustainably maintained by community members.

## ISLE Steering Committee
- Sarah Goldstein, Mount Holyoke College
- David Keiser-Clark (Chair), Williams College
- Francesca Livermore, Wesleyan University
- Mark McFate, Grinnell College
- Carolyn Moritz, Vassar College
- Ben Rosner, Barnard College
- Steve Young, Hamilton College

## ISLE Project Maintainers
- Ben Rosner (Lead), Barnard College
- Carolyn Moritz, Vassar College
- Mark Sandford (Lead), Colgate University
- Francesca Livermore, Wesleyan University
- David Keiser-Clark, Williams College

## ISLE Funding Institutions
* Amherst College
* Barnard College
* California Institute of Technology
* Colgate University
* Grinnell College
* Hamilton College
* Hampshire College
* Mount Holyoke College
* Rensselaer Polytechnic Institute
* Smith College
* St. Lawrence University
* Tulane University
* University of Manitoba Libraries
* University of Pittsburgh
* Vassar College
* Wesleyan University
* Williams College
* Born-Digital
* Islandora Foundation (Board supports ISLE; not a funder)
