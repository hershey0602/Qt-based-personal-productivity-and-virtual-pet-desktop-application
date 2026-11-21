from pathlib import Path
import json,re,xml.etree.ElementTree as ET,zipfile
base=Path(__file__).resolve().parents[1]
root=next(p for p in base.parents if (p/'FocusGarden/Qt_UI_Implementation_Blueprint/API_REGISTRY.json').exists())
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path,default=Path.cwd()/'build'/'audit');args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
reg=json.loads((root/'FocusGarden/Qt_UI_Implementation_Blueprint/API_REGISTRY.json').read_text(encoding='utf-8-sig'))['ports']
checks=[]
for name,port in reg.items():
 p=base/('src/contracts/ServicePorts.h' if name.startswith('I') else 'src/services/'+name+'.h')
 text=p.read_text(encoding='utf-8'); missing=[]
 for sig in [m[0] for m in port['methods']]+port['signals']:
  s=sig.rstrip(';')
  if s not in text:missing.append(sig)
 checks.append(dict(port=name,methods=len(port['methods']),signals=len(port['signals']),missing=missing))
qrc=ET.parse(root/'FocusGarden/resources.qrc')
assets=[n.text for n in qrc.iter('file')]
report={'version':'FG-API-1.0','ports':checks,'qrc_count':len(assets),'missing_assets':[a for a in assets if not (root/'FocusGarden'/a).is_file()]}
(args.output/'contract_audit.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
assert not any(c['missing'] for c in checks),report
assert not report['missing_assets']
print('PASS:',len(checks),'port/concrete declarations;',sum(c['methods'] for c in checks),'methods;',len(assets),'resources')
