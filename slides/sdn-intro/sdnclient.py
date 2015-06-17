import httplib2
import json
import pprint
 
h = httplib2.Http(".cache")
h.add_credentials('admin', 'admin')

resp, content = h.request('http://127.0.0.1:8080/controller/nb/v2/topology/default/', "GET")
topology = json.loads(content)

print "Topologie : "
pprint.pprint(topology)

resp, content = h.request('http://127.0.0.1:8080/controller/nb/v2/statistics/default/flow', "GET")
statistics = json.loads(content)

print "--------------------------------"
print "Statistiques : "
pprint.pprint(statistics)
