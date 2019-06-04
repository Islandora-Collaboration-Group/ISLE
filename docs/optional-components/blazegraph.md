# Blazegraph

## What is this component?

[Blazegraph™](https://www.blazegraph.com/) is a ultra high-performance graph database supporting Apache TinkerPop™ and RDF/SPARQL APIs. It supports up to 50 Billion edges on a single machine. Formerly known as BigData.

## Why use this component with ISLE?

Islandora users who have Fedora repositories with over 600K~ objects ingested have reported issues with using the default Mulgara triplestore used for indexing objects ingested into the Fedora repository. These issues include crashes, extreme performance slowdowns,system timeouts and more. Blazegraph is used to replace Mulgara as the triplestore and to deliver a higher level of performance for larger Fedora repositories.

* How can I tell how many Fedora objects I have?

## Requirements

* ISLE running system using `dashboards-dev` images from Docker-Hub.
  * List here:

* ISLE configuration from `dashboards-dev` branch on Born-Digital Fork

 * `git clone -b dashboards-dev

---

## Assumptions

* Assumes you have a running ISLE system using the `dashboards-dev` images

* Assumes you've configured your ISLE system to use Blazegraph

* Assumes port 8084 is not open to the public Internet only to select trusted administrators.

---

## Installation / Use with ISLE

*
*
*

---

## Installation

*
*

---

### Docker-compose.yml edits

*
*

---

## Troubleshooting

*
*

---

## Release Notes

*
*

---

## Additional Resources

### Check the Blazegraph triples count

Checking the triples count is an easy way to double-check if objects are being indexed properly and added to Blazegraph's triplestore.

To check the triples count in Blazegraph:

* Navigate to the Blazegraph Admin panel http://isle.localdomain:8084/blazegraph/#query
  * Replace `isle.localdomain` with your real domain when using in `Staging` or `Production`

* Copy and paste the following query into the field with the text (_Input a SPARQL query_):

```bash
SELECT (COUNT(*) AS ?triples) WHERE {?s ?p ?o}
```

* Click the `Execute` button.

* The output at the bottom will be a numerical value that will look something like:

```bash
-----------
| triples |
| ------- |
| 8280864 |
-----------
```

* This value should increase as more objects are ingested and indexed by Fedora. The difference is that instead of these values being in the previously used Mulgara triplestore, they are now handled by Blazegraph.