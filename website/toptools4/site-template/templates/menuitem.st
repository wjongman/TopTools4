$if(it.selected)$
<div class="selmenuitem">$it.caption$</div>
$it.submenu:submenuitem()$
$else$
<div class="menuitem"><a href="$it.pageurl$">$it.caption$</a></div>
$endif$


