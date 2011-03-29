<html>
<head>
  <title>$title$</title>
  <link rel="stylesheet" type="text/css" href="style.css" title="style">
</head>
  <body>
  <table class="banner">
    <tr>
      <td><h1>$title$</h1></td>
    </tr>
  </table>
<!--   <table border="1" width="600"> -->
  <table class="main">
    <tr>
      <td valign="top">
        <table class="menu">
          $menuitems:menuitem()$
        </table>
      </td>
      <td>&nbsp</td>
      <td valign="top" colspan="2"><h3>$subtitle$</h3>
        <div class="content">
          $content$
        </div>
      </td>
    </tr>
  </table>
  </body>
</html>

