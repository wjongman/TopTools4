$if(it.selected)$
<tr>
  <td colspan="2"><div class="selmenuitem">$it.caption$</div></td>
</tr>
<tr>
  <td>&nbsp;&nbsp;</td>
  <td><div class="submenuitem"><a href="$it.downloadurl$">Download</a></div></td>
</tr>
$else$
<tr>
  <td colspan="2"><div class="menuitem"><a href="$it.pageurl$">$it.caption$</a></div></td>
</tr>
$endif$

