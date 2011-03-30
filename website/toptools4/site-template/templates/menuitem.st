$if(it.selected)$
<div class="selmenuitem">$it.caption$</div>
<div class="submenuitem"><a href="$it.downloadurl$">download</a></div>
$else$
<div class="menuitem"><a href="$it.pageurl$">$it.caption$</a></div>
$endif$

